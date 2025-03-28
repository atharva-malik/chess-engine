#include <matchmaking/sprt/sprt.hpp>

#include <cmath>
#include <iomanip>
#include <sstream>

#include <core/logger/logger.hpp>
#include <matchmaking/stats.hpp>

namespace fastchess {

SPRT::SPRT(double alpha, double beta, double elo0, double elo1, std::string model, bool enabled) {
    enabled_ = enabled;

    if (enabled_) {
        lower_ = std::log(beta / (1 - alpha));
        upper_ = std::log((1 - beta) / alpha);

        elo0_ = elo0;
        elo1_ = elo1;

        model_ = model;

        Logger::trace("Initialized valid SPRT configuration.");
    }
}

double SPRT::leloToScore(double lelo) noexcept { return 1 / (1 + std::pow(10, (-lelo / 400))); }

double SPRT::bayeseloToScore(double bayeselo, double drawelo) noexcept {
    double pwin  = 1.0 / (1.0 + std::pow(10.0, (-bayeselo + drawelo) / 400.0));
    double ploss = 1.0 / (1.0 + std::pow(10.0, (bayeselo + drawelo) / 400.0));
    double pdraw = 1.0 - pwin - ploss;

    return pwin + 0.5 * pdraw;
}

double SPRT::neloToScoreWDL(double nelo, double variance) noexcept {
    return nelo * std::sqrt(variance) / (800.0 / std::log(10)) + 0.5;
}

double SPRT::neloToScorePenta(double nelo, double variance) noexcept {
    return nelo * std::sqrt(2.0 * variance) / (800.0 / std::log(10)) + 0.5;
}

void SPRT::isValid(double alpha, double beta, double elo0, double elo1, std::string model, bool& report_penta) {
    if (elo0 >= elo1) {
        throw std::runtime_error("Error; SPRT: elo0 must be less than elo1!");
    } else if (alpha <= 0 || alpha >= 1) {
        throw std::runtime_error("Error; SPRT: alpha must be a decimal number between 0 and 1!");
    } else if (beta <= 0 || beta >= 1) {
        throw std::runtime_error("Error; SPRT: beta must be a decimal number between 0 and 1!");
    } else if (alpha + beta >= 1) {
        throw std::runtime_error("Error; SPRT: sum of alpha and beta must be less than 1!");
    } else if (model != "normalized" && model != "bayesian" && model != "logistic") {
        throw std::runtime_error("Error; SPRT: invalid SPRT model!");
    }

    if (model == "bayesian" && report_penta) {
        Logger::warn(
            "Warning: Bayesian SPRT model not available with pentanomial statistics. Disabling "
            "pentanomial reports...");
        report_penta = false;
    }
}

double SPRT::getLLR(const Stats& stats, bool penta) const noexcept {
    if (penta)
        return getLLR(stats.penta_WW, stats.penta_WD, stats.penta_WL, stats.penta_DD, stats.penta_LD, stats.penta_LL);

    return getLLR(stats.wins, stats.draws, stats.losses);
}

double SPRT::getLLR(int win, int draw, int loss) const noexcept {
    if (!enabled_) return 0.0;

    const bool regularize = ((win == 0) + (draw == 0) + (loss == 0)) >= 2;
    const double games    = win + draw + loss + 1.5 * regularize;

    if (games == 0) return 0.0;

    const double W = (win + 0.5 * regularize) / games;
    const double D = (draw + 0.5 * regularize) / games;
    const double L = (loss + 0.5 * regularize) / games;

    double score0;
    double score1;

    if (model_ == "normalized") {
        const double score    = W + 0.5 * D;
        const double W_dev    = W * std::pow((1 - score), 2);
        const double D_dev    = D * std::pow((0.5 - score), 2);
        const double L_dev    = L * std::pow((0 - score), 2);
        const double variance = W_dev + D_dev + L_dev;
        if (variance == 0) return 0.0;
        score0 = neloToScoreWDL(elo0_, variance);
        score1 = neloToScoreWDL(elo1_, variance);
    } else if (model_ == "bayesian") {
        if (win == 0 || loss == 0) return 0.0;
        const double drawelo = 200 * std::log10((1 - L) / L * (1 - W) / W);
        score0               = bayeseloToScore(elo0_, drawelo);
        score1               = bayeseloToScore(elo1_, drawelo);
    } else {
        score0 = leloToScore(elo0_);
        score1 = leloToScore(elo1_);
    }

    const double W_dev0    = W * std::pow((1 - score0), 2);
    const double D_dev0    = D * std::pow((0.5 - score0), 2);
    const double L_dev0    = L * std::pow((0 - score0), 2);
    const double variance0 = W_dev0 + D_dev0 + L_dev0;
    const double W_dev1    = W * std::pow((1 - score1), 2);
    const double D_dev1    = D * std::pow((0.5 - score1), 2);
    const double L_dev1    = L * std::pow((0 - score1), 2);
    const double variance1 = W_dev1 + D_dev1 + L_dev1;

    if (variance0 == 0 || variance1 == 0) return 0.0;

    // For more information: http://hardy.uhasselt.be/Fishtest/support_MLE_multinomial.pdf
    return 0.5 * games * std::log(variance0 / variance1);
}

double SPRT::getLLR(int penta_WW, int penta_WD, int penta_WL, int penta_DD, int penta_LD, int penta_LL) const noexcept {
    if (!enabled_) return 0.0;

    const bool regularize =
        ((penta_WW == 0) + (penta_WD == 0) + ((penta_WL + penta_DD) == 0) + (penta_LD == 0) + (penta_LL == 0)) >= 4;
    const double pairs = penta_WW + penta_WD + penta_WL + penta_DD + penta_LD + penta_LL + 2.5 * regularize;

    if (pairs == 0) return 0.0;

    const double WW = (penta_WW + 0.5 * regularize) / pairs;
    const double WD = (penta_WD + 0.5 * regularize) / pairs;
    const double WL = (penta_WL + 0.25 * regularize) / pairs;
    const double DD = (penta_DD + 0.25 * regularize) / pairs;
    const double LD = (penta_LD + 0.5 * regularize) / pairs;
    const double LL = (penta_LL + 0.5 * regularize) / pairs;

    double score0;
    double score1;

    if (model_ == "normalized") {
        const double score    = WW + 0.75 * WD + 0.5 * (WL + DD) + 0.25 * LD;
        const double WW_dev   = WW * std::pow((1 - score), 2);
        const double WD_dev   = WD * std::pow((0.75 - score), 2);
        const double WLDD_dev = (WL + DD) * std::pow((0.5 - score), 2);
        const double LD_dev   = LD * std::pow((0.25 - score), 2);
        const double LL_dev   = LL * std::pow((0 - score), 2);
        const double variance = WW_dev + WD_dev + WLDD_dev + LD_dev + LL_dev;

        if (variance == 0) return 0.0;

        score0 = neloToScorePenta(elo0_, variance);
        score1 = neloToScorePenta(elo1_, variance);
    } else {
        score0 = leloToScore(elo0_);
        score1 = leloToScore(elo1_);
    }

    const double WW_dev0   = WW * std::pow((1 - score0), 2);
    const double WD_dev0   = WD * std::pow((0.75 - score0), 2);
    const double WLDD_dev0 = (WL + DD) * std::pow((0.5 - score0), 2);
    const double LD_dev0   = LD * std::pow((0.25 - score0), 2);
    const double LL_dev0   = LL * std::pow((0 - score0), 2);
    const double variance0 = WW_dev0 + WD_dev0 + WLDD_dev0 + LD_dev0 + LL_dev0;
    const double WW_dev1   = WW * std::pow((1 - score1), 2);
    const double WD_dev1   = WD * std::pow((0.75 - score1), 2);
    const double WLDD_dev1 = (WL + DD) * std::pow((0.5 - score1), 2);
    const double LD_dev1   = LD * std::pow((0.25 - score1), 2);
    const double LL_dev1   = LL * std::pow((0 - score1), 2);
    const double variance1 = WW_dev1 + WD_dev1 + WLDD_dev1 + LD_dev1 + LL_dev1;

    if (variance0 == 0 || variance1 == 0) return 0.0;

    // For more information: http://hardy.uhasselt.be/Fishtest/support_MLE_multinomial.pdf
    return 0.5 * pairs * std::log(variance0 / variance1);
}

SPRTResult SPRT::getResult(double llr) const noexcept {
    if (!enabled_) return SPRT_CONTINUE;

    if (llr >= upper_)
        return SPRT_H0;
    else if (llr <= lower_)
        return SPRT_H1;
    return SPRT_CONTINUE;
}

std::string SPRT::getBounds() const noexcept {
    std::stringstream ss;

    ss << "(" << std::fixed << std::setprecision(2) << lower_ << ", " << std::fixed << std::setprecision(2) << upper_
       << ")";

    return ss.str();
}

std::string SPRT::getElo() const noexcept {
    std::stringstream ss;

    ss << "[" << std::fixed << std::setprecision(2) << elo0_ << ", " << std::fixed << std::setprecision(2) << elo1_
       << "]";

    return ss.str();
}

double SPRT::getLowerBound() const noexcept { return lower_; }

double SPRT::getUpperBound() const noexcept { return upper_; }

bool SPRT::isEnabled() const noexcept { return enabled_; }

}  // namespace fastchess
