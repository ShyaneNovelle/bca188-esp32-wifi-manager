# Laboratory Report: ESP32 Wi-Fi Manager

**Laboratory Activity 9 (Individual)**

| | |
|---|---|
| **Name** | [Your Full Name] |
| **Assigned number** | 2024-2083 |
| **Course** | BCA188 |
| **Date performed** | 24 October 2026 |
| **Board** | ESP32-WROOM-DA Module, connected on COM4 |
| **Software** | Arduino IDE 2.3.10, ESP32 board support (Espressif Systems), WiFiManager library by tzapu |

---

## 1. Objectives

1. Set up Wi-Fi Manager on an ESP32.
2. Connect an ESP32 to different Wi-Fi networks without reprogramming it.

## 2. Materials

| Item | Used in this activity |
|------|-----------------------|
| ESP32 board | ESP32-WROOM-DA Module |
| USB cable | Connects the board to the computer for uploading and the Serial Monitor |
| Computer with Arduino IDE | Laptop with Arduino IDE 2.3.10 |
| WiFiManager library (by tzapu) | Installed from the Library Manager |
| Phone | Used to join the ESP32 setup network and open the setup page; also provided the mobile hotspot |
| Two 2.4 GHz networks | Home router (`W.I.F.I [E.E]`) and phone hotspot (`Hammurabi Code`, 2.4 GHz) |

## 3. Background: how WiFiManager works

WiFiManager removes the need to write the Wi-Fi name and password into the code. When the ESP32 starts:

1. It tries to connect to the Wi-Fi network it has saved in flash memory.
2. If there is no saved network, or the connection fails, it starts its own Wi-Fi network (an access point) with the name given in the sketch.
3. A phone or laptop joins that network and opens `192.168.4.1`, where WiFiManager shows a setup page listing nearby networks.
4. The user enters the Wi-Fi name (SSID) and password. The ESP32 saves them, tries to connect, and closes the setup portal if it succeeds.

Because the credentials are saved on the board and entered at runtime, the same program can be used on any 2.4 GHz network.

## 4. Procedure

1. **Setup:** I installed ESP32 board support in the Arduino IDE and the WiFiManager library by tzapu from the Library Manager.
2. **Connect hardware:** I plugged the ESP32 into the computer and selected the ESP32-WROOM-DA Module and port COM4.
3. **Program the ESP32:** I used the class sketch, set the network name to `BCA188-ESP32-2024-2083`, and added `wm.setConfigPortalTimeout(180);` before `wm.autoConnect()`.
4. **Use it:** I uploaded the code, opened the Serial Monitor at 115200 baud, joined the ESP32's network with my phone, opened `192.168.4.1`, entered my Wi-Fi details, and noted the IP address printed in the Serial Monitor.

### Key parts of the sketch

The complete sketch is in [`esp32_wifimanager/esp32_wifimanager.ino`](esp32_wifimanager/esp32_wifimanager.ino).

| Code | Purpose |
|------|---------|
| `const char* AP_NAME = "BCA188-ESP32-2024-2083";` | Name of the setup Wi-Fi network the ESP32 creates |
| `WiFi.mode(WIFI_STA);` | Puts the ESP32 in station mode before WiFiManager starts |
| `wm.setConfigPortalTimeout(180);` | Closes the setup portal after 180 seconds (3 minutes) so the board does not stay stuck |
| `wm.autoConnect(AP_NAME);` | Tries the saved Wi-Fi first; if that fails, starts the setup network and portal at `192.168.4.1` |
| `if (!connected) { ESP.restart(); }` | Restarts the board when setup times out or the connection fails |
| `wm.resetSettings();` in `loop()` | Erases the saved Wi-Fi when the BOOT button (GPIO0) is held for 3 seconds |

## 5. Initial setup and first connection

After uploading the code, the ESP32 started its setup network. My phone showed `BCA188-ESP32-2024-2083` in its list of available Wi-Fi networks (Figure 1), and the setup page opened at `192.168.4.1` (Figure 2). The Serial Monitor printed `16 networks found`, which is the scan of nearby networks that fills the list on the setup page (Figure 3).

**Figure 1.** The ESP32 setup network `BCA188-ESP32-2024-2083` in my phone's Wi-Fi list.

<img src="images/01-esp32-network-in-phone-wifi-list.jpg" alt="Phone Wi-Fi list showing BCA188-ESP32-2024-2083" width="300">

**Figure 2.** The WiFiManager setup page at `192.168.4.1`, opened on my phone ("No AP set" means no Wi-Fi was saved yet).

<img src="images/02-setup-page-on-phone.jpg" alt="WiFiManager setup page on phone" width="300">

**Figure 3.** Serial Monitor after the first start, showing `16 networks found`.

![Serial Monitor: 16 networks found](images/03-serial-networks-found.png)

## 6. Practice tasks

### Summary of results

| Task | What to do | What I observed |
|------|------------|-----------------|
| 1. Wrong password | Enter a wrong password on the setup page. | The connection failed (`[ERROR] Connect to new AP Failed`). The board restarted, retried the saved credentials, failed again, and opened the setup network again. |
| 2. Reset settings | Hold the BOOT button for 3 seconds while the board is running. | Saved Wi-Fi was erased (`No wifi saved, skipping`) and the setup network started again. |
| 3. Change the name | Change the network name, upload, and check the phone's Wi-Fi list. | The new name `IamSagi` appeared in the phone's Wi-Fi list. |
| 4. Different network | Reset the board, then connect to the phone hotspot. | The board connected to `Hammurabi Code` and received IP `10.22.84.193` (previously `192.168.0.103` on the home router). |

### Task 1: Wrong password

**What I did:** On the setup page I selected my home network `W.I.F.I [E.E]` and entered a wrong password.

**What I observed:**

- The phone showed "Saving Credentials – Trying to connect ESP to network. If it fails reconnect to AP to try again" (Figure 4).
- The Serial Monitor printed `Connecting to NEW AP: W.I.F.I [E.E]` and waited for the result (Figure 5), then printed `[ERROR] Connect to new AP Failed` (Figure 6).
- Setup was not completed, so the sketch printed `Setup timed out or connection failed. Restarting...` and the board rebooted.
- After rebooting, it tried the saved (wrong) credentials (`Connecting to SAVED AP`), which failed after about 60 seconds (`AutoConnect: FAILED for 60503 ms`). It then started the setup network and portal again (Figure 7).
- I entered the correct password, and the board connected: `Connect to new AP [SUCCESS]`, `Connected!`, `Network: W.I.F.I [E.E]`, `IP address: 192.168.0.103` (Figure 7).

The timeout worked as intended: a wrong password did not leave the board stuck, because it restarted and offered the setup network again.

**Figure 4.** Phone: "Saving Credentials" page after submitting the wrong password.

<img src="images/04-task1-phone-saving-credentials.jpg" alt="Phone showing Saving Credentials page" width="300">

**Figure 5.** Serial Monitor: connecting to `W.I.F.I [E.E]` with the wrong password.

![Serial Monitor: connecting to new AP](images/05-task1-serial-connecting.png)

**Figure 6.** Serial Monitor: `[ERROR] Connect to new AP Failed`.

![Serial Monitor: connect to new AP failed](images/06-task1-serial-connect-failed.png)

**Figure 7.** Serial Monitor: restart after the failed setup, setup portal started again, then connected with the correct password (IP `192.168.0.103`).

![Serial Monitor: restart, new setup, connected with IP address](images/07-serial-restart-then-connected-ip.png)

### Task 2: Reset settings

**What I did:** While the board was running and connected, I held the BOOT button for 3 seconds.

**What I observed:** The board erased the saved Wi-Fi settings and restarted. After the restart, the Serial Monitor printed `AutoConnect`, `No wifi saved, skipping`, and `AutoConnect: FAILED for 1 ms`. The board then started the setup network `BCA188-ESP32-2024-2083` with the portal at `192.168.4.1` instead of reconnecting to my router (Figure 8).

**Figure 8.** Serial Monitor after the BOOT reset: no Wi-Fi saved, setup network started.

![Serial Monitor: No wifi saved, StartAP](images/08-task2-serial-no-wifi-saved.png)

### Task 3: Change the name

**What I did:** I changed `AP_NAME` in the sketch from `BCA188-ESP32-2024-2083` to `IamSagi` (Figure 9), uploaded it, and checked my phone's Wi-Fi list.

**What I observed:** The Serial Monitor printed `StartAP with SSID: IamSagi` (Figure 10), and the new name `IamSagi` appeared in my phone's Wi-Fi list (Figure 11). My phone connected to it and showed "Connected without internet / Not secured", which is normal because the ESP32's setup network is open and has no internet access.

The new name appeared immediately because the saved Wi-Fi had already been erased in Task 2 (`No wifi saved, skipping`). Uploading new code does not erase saved Wi-Fi credentials, so if the board had still been connected to a router, it would have reconnected to it and would not have shown the new name until it was reset with the BOOT button.

**Figure 9.** The sketch with `AP_NAME` changed to `IamSagi`.

![Code with AP_NAME set to IamSagi](images/09-task3-code-new-ap-name.png)

**Figure 10.** Serial Monitor: `StartAP with SSID: IamSagi`.

![Serial Monitor: StartAP with SSID IamSagi](images/10-task3-serial-new-ap-name.png)

**Figure 11.** Phone Wi-Fi list showing the new name `IamSagi`.

<img src="images/11-task3-phone-wifi-list-new-name.jpg" alt="Phone Wi-Fi list showing IamSagi" width="300">

### Task 4: Different network

**What I did:** Following the task, I reset the board with the BOOT button so it had no saved Wi-Fi. I turned on my phone's Mobile Hotspot (`Hammurabi Code`, 2.4 GHz band; Figure 12). Because the hotspot phone cannot also be used to configure the board, I used my laptop to join the ESP32's setup network, opened `192.168.4.1`, and entered the hotspot's name and password (Figure 13).

**What I observed:** The board connected to the hotspot without any code change. The Serial Monitor printed `Connect to new AP [SUCCESS]`, `Connected!`, `Network: Hammurabi Code`, and `IP address: 10.22.84.193` (Figure 14). The phone's hotspot page lists the ESP32 (`esp32-A359E0`) as 1 connected device (Figure 12).

**Figure 12.** Phone Mobile Hotspot settings: network `Hammurabi Code`, 2.4 GHz band, and the ESP32 shown as a connected device. (The hotspot password has been hidden.)

<img src="images/12-task4-phone-hotspot-settings.jpg" alt="Phone Mobile Hotspot settings with ESP32 connected" width="300">

**Figure 13.** Laptop browser at `192.168.4.1/wifisave` after entering the hotspot details.

![Laptop showing Saving Credentials page](images/13-task4-laptop-saving-credentials.png)

**Figure 14.** Serial Monitor: connected to `Hammurabi Code` with IP `10.22.84.193`.

![Serial Monitor: connected to Hammurabi Code, IP 10.22.84.193](images/14-task4-serial-connected-hotspot.png)

## 7. Comparison of the two networks

| | Home router | Phone hotspot |
|---|---|---|
| **Network name** | `W.I.F.I [E.E]` | `Hammurabi Code` |
| **IP address of the ESP32** | `192.168.0.103` | `10.22.84.193` |
| **Code changed?** | No | No |
| **How the details were entered** | Setup page on the phone | Setup page on the laptop |

## 8. Discussion

**Why did the board connect to a different network without a code change?** WiFiManager stores the Wi-Fi name and password in the ESP32's flash memory and lets the user enter new ones through the setup page at runtime. Nothing about the network is hard-coded in the sketch, so the same program works with any 2.4 GHz network. After a reset, the board goes back to the setup portal and can be given a new network.

**Why are the IP addresses different?** Each network has its own DHCP server that gives out addresses from its own range. The home router gave `192.168.0.103` and the phone hotspot gave `10.22.84.193`, which shows the board really did join a different network.

**Why use a timeout?** A wrong password does not permanently block the board. With `wm.setConfigPortalTimeout(180);` and the restart in the sketch, the board restarts after the setup window passes with no successful setup, then tries again and offers the setup network again (Task 1).

**Saved settings and uploads.** Uploading new code does not erase the saved Wi-Fi credentials. To make the board show its setup network again (for example, to see a new `AP_NAME`), the settings must be reset with the BOOT button, unless nothing is saved yet (Task 3).

## 9. Problems encountered

| Problem | What I did |
|---------|-----------|
| The phone cannot be both the hotspot and the device that configures the board. | I used my laptop to join the ESP32 setup network and open `192.168.4.1` while the phone provided the hotspot. |
| The phone showed "No internet" / "Connected without internet" on the ESP32 setup network. | This is expected because the ESP32's setup network has no internet access. I stayed connected and opened `192.168.4.1` manually. |
| The ESP32 only supports 2.4 GHz networks. | I used a 2.4 GHz network for the home router and set the phone hotspot to the 2.4 GHz band. |

## 10. Conclusion

I set up WiFiManager on an ESP32 and used it to connect the board to two different networks, my home router and my phone hotspot, without changing the code. I observed what happens with a wrong password (the board restarts and offers the setup network again), how the BOOT button resets the saved Wi-Fi, and that a new setup network name appears after a reset. The activity showed how storing credentials on the board and entering them through a setup portal makes an ESP32 project much easier to move between networks.

## Appendix: Figure index

| Figure | File |
|--------|------|
| 1 | `images/01-esp32-network-in-phone-wifi-list.jpg` |
| 2 | `images/02-setup-page-on-phone.jpg` |
| 3 | `images/03-serial-networks-found.png` |
| 4 | `images/04-task1-phone-saving-credentials.jpg` |
| 5 | `images/05-task1-serial-connecting.png` |
| 6 | `images/06-task1-serial-connect-failed.png` |
| 7 | `images/07-serial-restart-then-connected-ip.png` |
| 8 | `images/08-task2-serial-no-wifi-saved.png` |
| 9 | `images/09-task3-code-new-ap-name.png` |
| 10 | `images/10-task3-serial-new-ap-name.png` |
| 11 | `images/11-task3-phone-wifi-list-new-name.jpg` |
| 12 | `images/12-task4-phone-hotspot-settings.jpg` |
| 13 | `images/13-task4-laptop-saving-credentials.png` |
| 14 | `images/14-task4-serial-connected-hotspot.png` |
