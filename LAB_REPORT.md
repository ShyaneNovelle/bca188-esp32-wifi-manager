<img width="720" height="1600" alt="bafef874-4319-4852-b588-27e85dbe53de" src="https://github.com/user-attachments/assets/5d9ef8f5-6739-4695-a8ce-a7b13decd5cc" /># Laboratory Report: ESP32 Wi-Fi Manager

**Laboratory Activity 9 **

| | |
|---|---|
| **Name** | Shyane Novelle R. Canayan |
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

The complete sketch is in 
(esp32_wifi_manager.ino

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

<img width="720" height="1600" alt="bafef874-4319-4852-b588-27e85dbe53de" src="https://github.com/user-attachments/assets/797830cd-acd5-41d0-aa32-d1f9909179ea" />


**Figure 2.** The WiFiManager setup page at `192.168.4.1`, opened on my phone ("No AP set" means no Wi-Fi was saved yet).

<img width="720" height="1600" alt="4376a0af-b038-44b0-b785-d33b6862f173" src="https://github.com/user-attachments/assets/64ce1610-df5b-4735-b63b-082c1b05d283" />


**Figure 3.** Serial Monitor after the first start, showing `16 networks found`.

<img width="640" height="392" alt="Screenshot 2026-10-24 120805" src="https://github.com/user-attachments/assets/75ef466e-3547-45de-8b4d-51e80ab80b82" />


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

<img width="720" height="1600" alt="bafef874-4319-4852-b588-27e85dbe53de" src="https://github.com/user-attachments/assets/b4cb883a-4916-4046-a1bf-25b9f6818d9b" />


**Figure 5.** Serial Monitor: connecting to `W.I.F.I [E.E]` with the wrong password.

<img width="635" height="368" alt="Screenshot 2026-10-24 121111" src="https://github.com/user-attachments/assets/6569dd96-a808-4a05-bc28-96429e1a92cf" />


**Figure 6.** Serial Monitor: `[ERROR] Connect to new AP Failed`.
Serial Monitor: restart after the failed setup, setup portal started again, then connected with the correct password (IP `192.168.0.103`).

<img width="640" height="392" alt="Screenshot 2026-10-24 120805" src="https://github.com/user-attachments/assets/6777e8c0-b857-4314-a590-662e065eb130" />


### Task 2: Reset settings

**What I did:** While the board was running and connected, I held the BOOT button for 3 seconds.

**What I observed:** The board erased the saved Wi-Fi settings and restarted. After the restart, the Serial Monitor printed `AutoConnect`, `No wifi saved, skipping`, and `AutoConnect: FAILED for 1 ms`. The board then started the setup network `BCA188-ESP32-2024-2083` with the portal at `192.168.4.1` instead of reconnecting to my router (Figure 8).

**Figure 7.** Serial Monitor after the BOOT reset: no Wi-Fi saved, setup network started.

<img width="640" height="353" alt="Screenshot 2026-10-24 122627" src="https://github.com/user-attachments/assets/e04376d3-dfec-4d06-be24-a58d2f41349c" />


### Task 3: Change the name

**What I did:** I changed `AP_NAME` in the sketch from `BCA188-ESP32-2024-2083` to `IamSagi` (Figure 9), uploaded it, and checked my phone's Wi-Fi list.

**What I observed:** The Serial Monitor printed `StartAP with SSID: IamSagi` (Figure 10), and the new name `IamSagi` appeared in my phone's Wi-Fi list (Figure 11). My phone connected to it and showed "Connected without internet / Not secured", which is normal because the ESP32's setup network is open and has no internet access.

The new name appeared immediately because the saved Wi-Fi had already been erased in Task 2 (`No wifi saved, skipping`). Uploading new code does not erase saved Wi-Fi credentials, so if the board had still been connected to a router, it would have reconnected to it and would not have shown the new name until it was reset with the BOOT button.

**Figure 8.** The sketch with `AP_NAME` changed to `IamSagi`.

<img width="720" height="1600" alt="7299ca76-a893-4000-95f9-d794f4d93491" src="https://github.com/user-attachments/assets/d0d9d3fc-1433-4ff0-bcfa-87de7ad48dac" />


**Figure 9.** Serial Monitor: `StartAP with SSID: IamSagi`.

<img width="635" height="327" alt="Screenshot 2026-10-24 123707" src="https://github.com/user-attachments/assets/18af2e3a-5e7c-40b2-831a-668cdee415b5" />


**Figure 10.** Phone Wi-Fi list showing the new name `IamSagi`.

<img width="720" height="1600" alt="382d5fe6-0177-4333-b107-ef75add95439" src="https://github.com/user-attachments/assets/d15b3bca-d56a-48fd-a3c4-468ac635920f" />


### Task 4: Different network

**What I did:** Following the task, I reset the board with the BOOT button so it had no saved Wi-Fi. I turned on my phone's Mobile Hotspot (`Hammurabi Code`, 2.4 GHz band; Figure 12). Because the hotspot phone cannot also be used to configure the board, I used my laptop to join the ESP32's setup network, opened `192.168.4.1`, and entered the hotspot's name and password (Figure 13).

**What I observed:** The board connected to the hotspot without any code change. The Serial Monitor printed `Connect to new AP [SUCCESS]`, `Connected!`, `Network: Hammurabi Code`, and `IP address: 10.22.84.193` (Figure 14). The phone's hotspot page lists the ESP32 (`esp32-A359E0`) as 1 connected device (Figure 12).

**Figure 11.** Phone Mobile Hotspot settings: network `Hammurabi Code`, 2.4 GHz band, and the ESP32 shown as a connected device. (The hotspot password has been hidden.)

<img width="720" height="1600" alt="7299ca76-a893-4000-95f9-d794f4d93491" src="https://github.com/user-attachments/assets/6dc51e47-f151-4e9f-a7e2-1c00544528bc" />


**Figure 12.** Laptop browser at `192.168.4.1/wifisave` after entering the hotspot details.

<img width="302" height="158" alt="Screenshot 2026-10-24 133028" src="https://github.com/user-attachments/assets/e9f6b0e1-e174-44ee-92ab-08995618e707" />


**Figure 13.** Serial Monitor: connected to `Hammurabi Code` with IP `10.22.84.193`.

<img width="639" height="119" alt="Screenshot 2026-10-24 133211" src="https://github.com/user-attachments/assets/eabb341c-19af-4fcb-9acc-3c9e238e019e" />


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
