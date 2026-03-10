#ifndef _VIRTUAL_KEY_DEF_H_
#define _VIRTUAL_KEY_DEF_H_

#ifdef __linux__
#define VK_LBUTTON 0xFF01
#define VK_RBUTTON 0xFF02
#define VK_CANCEL 0xFF03
#define VK_MBUTTON 0xFF04
#define VK_SHIFT 0xFFE1
#define VK_CONTROL 0xFFE3
#define VK_MENU 0xFFE9
#define VK_RETURN 0xFF0D
#define VK_ESCAPE 0xFF1B
#define VK_DELETE 0xFF2E
#define VK_SPACE 0xFF20
#define VK_TAB 0xFF09
#define VK_NUMPAD0 0xFF60
#define VK_NUMPAD9 0xFF69
#define VK_OEM_COMMA 0xFFBC
#else
#define VK_LBUTTON          0x01
#define VK_RBUTTON          0x02
#define VK_CANCEL           0x03
#define VK_MBUTTON          0x04
#define VK_SHIFT            0x10
#define VK_CONTROL          0x11
#define VK_MENU             0x12
#define VK_RETURN           0x0D
#define VK_ESCAPE           0x1B
#define VK_DELETE           0x2E
#define VK_SPACE            0x20
#define VK_TAB              0x09
#define VK_NUMPAD0          0x60
#define VK_NUMPAD9          0x69
#define VK_OEM_COMMA        0xBC    
#endif
#endif