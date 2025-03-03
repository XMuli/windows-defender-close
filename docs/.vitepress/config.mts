import { defineConfig } from 'vitepress'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "Windows-Defender-Close",
  description: "[Minimalist PC Manager] Microsoft Defender Anti-Virus Force Enable/Disable (Fixes the inability to start Anti-Virus on some PC)",
  head: [['link', { rel: 'icon', href: '/logo/favicon.ico' }]],

  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    logo: '/logo/logo.svg',
    // siteTitle: 'Hello World',
    nav: [
      { text: 'Home', link: '/' },
      // { text: 'Examples', link: '/markdown-examples' }
    ],

    // sidebar: [
    //   {
    //     text: 'Examples',
    //     items: [
    //       { text: 'Markdown Examples', link: '/markdown-examples' },
    //       { text: 'Runtime API Examples', link: '/api-examples' }
    //     ]
    //   }
    // ],

    socialLinks: [
      { icon: 'github', link: 'https://github.com/XMuli/windows-defender-close' },
      { icon: 'gmail', link: 'https://mail.google.com/mail/u/0/?view=cm&fs=1&tf=1&source=mailto&to=xmulitech@gmail.com' },
    ],

    footer: {
      message: 'Enjoy it on Windows 10 / 11.',
      copyright: 'Copyright © 2022-2025 <a href="https://github.com/XMuli" target="_blank" rel="noopener">XMuli</a>'
    },

    editLink: {
      pattern: 'https://github.com/XMuli/WinHelper/edit/master/docs/:path',
      text: 'Edit this page on GitHub'
    },

    // 允许自定义上次更新的文本和日期格式。
    lastUpdated: {
      text: 'Updated at',
      formatOptions: {
        dateStyle: 'full',
        timeStyle: 'medium'
      }
    },

    // search: {
    //   provider: 'local'
    // },
  }
})
