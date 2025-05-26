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
    lastUpdated: {
      text: 'Updated at',
      formatOptions: {
        dateStyle: 'full'
      }
    },

    sidebar: {
      '/quickstart/': [
        {
          text: 'Quick Start',
          items: [
            { text: 'Quick Start', link: '/quickstart/quickstart' },
          ]
        },
        {
          text: 'Other Links',
          items: [
            { text: 'Dev Log', link: '/devlog/test' }, //todo fix this routing
            { text: 'Documentation', link: '/docs/introduction' }
          ]
        }
      ],

      '/devlog/': [
        {
          text: 'Dev Log',
          items: [
            { text: 'Quickstart', link: '/quickstart' },
            { text: 'Markdown Examples', link: '/markdown-examples' },
            { text: 'Runtime API Examples', link: '/api-examples' }
          ]
        },
        {
          text: 'Other Links',
          items: [
            { text: 'Quickstart', link: '/quickstart/quickstart' },
            { text: 'Documentation', link: '/docs/introduction' }
          ]
        }
      ],

      '/docs/': [
        {
          text: 'Documentation',
          items: [
            { text: 'Contributing', link: '/docs/contributing.md' },
            { text: 'Introduction', link: '/docs/introduction.md' },
          ]
        },
        {
          text: 'Files',
          items: [
            { text: 'main.cpp', link: '/docs/main' }, //todo fix this routing
            { text: 'Documentation', link: '/docs/introduction' }
          ]
        },
        {
          text: 'Other Links',
          items: [
            { text: 'Dev Log', link: '/devlog/test' }, //todo fix this routing
            { text: 'Documentation', link: '/docs/introduction' }
          ]
        }
      ]
    },

    socialLinks: [
      { icon: 'github', link: 'https://github.com/atharva-malik/chess-engine' }
    ]
  }
})
