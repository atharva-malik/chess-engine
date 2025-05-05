import { defineConfig } from 'vitepress'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "FuryEngine Docs",
  description: "The documentation for the open-source Fury Engine",
  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    nav: [
      { text: 'Home', link: '/' },
      { text: 'Docs', link: '/docs/introduction' }
    ],

    sidebar: [
      {
        text: 'Quick Start',
        items: [
          { text: 'Quick Start', link: '/quickstart/quickstart' },
        ]
      },
      {
        text: 'Dev Log',
        items: [
          { text: 'Quickstart', link: '/quickstart' },
          { text: 'Markdown Examples', link: '/markdown-examples' },
          { text: 'Runtime API Examples', link: '/api-examples' }
        ]
      },
      {
        text: 'Documentation',
        items: [
          { text: 'Contributing', link: '/docs/contributing.md' },
          { text: 'Introduction', link: '/docs/introduction.md' },
          { text: 'Markdown Examples', link: '/markdown-examples' },
          { text: 'Runtime API Examples', link: '/api-examples' }
        ]
      }
    ],

    socialLinks: [
      { icon: 'github', link: 'https://github.com/atharva-malik/chess-engine' }
    ]
  }
})
