---
layout: home
---
<script src="https://cdn.jsdelivr.net/npm/anchor-js/anchor.min.js"></script>

# Seader

{% responsive_image path: ./fake_screenshot.png %}

[Flipper Zero](https://flipperzero.one/) application (aka "fap").

## Download

### Relaese

Download release versions via the Flipper App Catalog (when released)

### Beta/Dev

Download builds based off git at [flipc](https://flipc.org/bettse/seader?branch=main)

### Old builds

- [seader](seader-API30.1.zip) (API 30.1, FW 0.85.2)
- [seader](seader-API28.2.zip) (API 28.2)
- [seader](seader-API26.3.zip) (API 26.3)
- [seader](seader-API26.0.zip) (API 26.0)
- [seader](seader-API23.1.zip) (API 23.1)
- [seader](seader-API23.0.zip) (API 23.0)
- [seader](seader-API22.0.zip) (API 22.0)
- [seader](seader-API21.0.zip) (API 21.0)
- [seader](seader-API20.0.zip) (API 20.0)

## Bugs

File issues in [GitHub](https://github.com/bettse/seader/issues).

## Hardware

Put **[SAM](https://www.cdw.com/product/hp-sim-for-hid-iclass-for-hip2-reader-security-sim/4854794)** into **[adapter](https://a.co/d/1E9Zk1h)** (because of chip on top) and plug into **[reader](https://www.mikroe.com/smart-card-2-click)**. Connect reader to Flipper Zero (See `Connections` below).

Alternatively, [NARD flipper add-on](https://github.com/killergeek/nard) works.

### Connections

| Smart Card 2 Click | Flipper     |
| ------------------ | ----------- |
| 5v                 | 1           |
| GND                | 8 / 11 / 18 |
| TX                 | 16          |
| RX                 | 15          |

## References

- [omnikey_5025_cl_software_developer_guide_mn_en](https://www.virtualsecurity.nl/amfile/file/download/file/18/product/1892/)
- [omnikey_5326_dfr_softwaredeveloperguide](https://www.hidglobal.com/sites/default/files/documentlibrary/omnikey_5326_dfr_softwaredeveloperguide.pdf)
- [omnikey_5027_software_developer_guide](https://www.hidglobal.com/sites/default/files/documentlibrary/omnikey_5027_software_developer_guide.pdf)
- [PLT-03362 A.0 - iCLASS Reader Writer Migration Application Note](http://web.archive.org/web/20230330180023/https://info.hidglobal.com/rs/289-TSC-352/images/PLT-03362%20A.0%20-%20iCLASS%20Reader%20Writer%20Migration%20Application%20Note.pdf)
- [HID SE reader消息模块的ANS.1 BER学习](https://blog.csdn.net/eyasys/article/details/8501200)

<script>
   anchors.add();
</script>
