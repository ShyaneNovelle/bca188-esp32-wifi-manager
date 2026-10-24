# Laboratory Activity 9: ESP32 Wi-Fi Manager (Individual)

| | |
|---|---|
| **Name** | [Your Full Name] |
| **Assigned number** | 2024-2083 |
| **Setup network name** | `BCA188-ESP32-2024-2083` |
| **Course** | BCA188 |
| **Date performed** | 24 October 2026 |
| **Hardware / software** | ESP32-WROOM-DA Module (COM4), Arduino IDE 2.3.10, WiFiManager library by tzapu |

## About this lab

In this activity I set up the **WiFiManager** library on an ESP32 so that the board can join different Wi-Fi networks **without being reprogrammed**. When the board has no working Wi-Fi saved, it creates its own setup network (`BCA188-ESP32-2024-2083`). I connect to it with a phone, open `192.168.4.1`, and enter the Wi-Fi name and password. The board saves these details and connects to that network.

The sketch also restarts the board if setup is not completed within 3 minutes (`wm.setConfigPortalTimeout(180);`), and holding the **BOOT** button for 3 seconds erases the saved Wi-Fi settings.

## Results

| Task | What to do | What you observe |
|------|------------|------------------|
| 1. Wrong password | Enter a wrong password on the setup page. | The phone showed "Saving Credentials – Trying to connect ESP to network." The Serial Monitor printed `Connecting to NEW AP: W.I.F.I [E.E]` and then `[ERROR] Connect to new AP Failed`, so the board did not connect. After the setup was not completed, it printed `Setup timed out or connection failed. Restarting...`, rebooted, tried the saved (wrong) credentials for about 60 seconds (`AutoConnect: FAILED for 60503 ms`), and opened the setup network again instead of staying stuck. I then entered the correct password and it connected (`192.168.0.103`). |
| 2. Reset settings | Hold the BOOT button for 3 seconds while the board is running. | The board erased its saved Wi-Fi settings and restarted. The Serial Monitor printed `No wifi saved, skipping`, and the board started the setup network `BCA188-ESP32-2024-2083` (portal at `192.168.4.1`) again instead of reconnecting to my router. |
| 3. Change the name | Change the network name, upload, and check your phone's Wi-Fi list. | I changed `AP_NAME` to `IamSagi` and uploaded. The new name `IamSagi` appeared in my phone's Wi-Fi list, and the Serial Monitor printed `StartAP with SSID: IamSagi`. This appeared right after the upload because the saved Wi-Fi had been erased in Task 2. If the board were still connected to a router, it would not show the new name until it was reset with the BOOT button. |
| 4. Different network | Reset the board, then connect to your phone hotspot. | After the reset, I turned on my phone hotspot (`Hammurabi Code`, 2.4 GHz) and used my laptop to join the setup network and open `192.168.4.1`. After I entered the hotspot's name and password, the board connected. The Serial Monitor shows `Network: Hammurabi Code` and `IP address: 10.22.84.193` (earlier on my home router it was `192.168.0.103`), and the phone's hotspot page lists the ESP32 as a connected device. |

## Screenshots

**1. Setup page on my phone** (`192.168.4.1`, served by the ESP32 while it was in setup mode):

<img src="images/02-setup-page-on-phone.jpg" alt="WiFiManager setup page on phone" width="300">

**2. Serial Monitor showing a connected state with an IP address:**

![Serial Monitor showing Connected and IP address 192.168.0.103](images/07-serial-restart-then-connected-ip.png)

The other screenshots (all four tasks, step by step) are in the [full laboratory report](LAB_REPORT.md).

## Why did the board connect to a different network without a code change?

The board connected to a different network without a code change because WiFiManager stores the Wi-Fi name and password in the ESP32's flash memory and lets me enter new ones through the setup page at runtime, so no network details are hard-coded in the sketch.

## Files in this repository

| File | Description |
|------|-------------|
| `README.md` | This file: summary, results table, screenshots, and explanation |
| `LAB_REPORT.md` | Full laboratory report with procedure, all screenshots, and discussion |
| `esp32_wifimanager/esp32_wifimanager.ino` | The Arduino sketch used in this activity |
| `images/` | Screenshots used in the README and report |
