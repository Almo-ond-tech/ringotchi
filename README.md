# ringotchi

<img width="705" height="1000" alt="ringotchi_zine3" src="https://github.com/user-attachments/assets/4447fd84-298e-4cd5-8750-81c09b223463" />



# Description
A tamagotchi pet with an apple shape. The usb port plugs into the top of the apple to form the stem. The sprite for the pet is a cat with glasses to match the case. And it has a pet worm because it lives in an apple. I made this following the tutorial for hackclub fallout: [https://fallout.hackclub.com/docs/guided-projects/tamagotchi].
I like retro toys and I'm glad I get to make my own custom tamagotchi that I can personalise to do stuff. I'm thinking of adding extra stuff like a death screen and mini game but I'll just wait to build it in real life first. I also used this github as reference to program the jingles: https://github.com/FeFaxy/arduino_buzzer/blob/main/Twinkle_Twinkle_Little_Star.ino


<h2> Functions</h2>

</ul>
<h3> DISPLAY </h3>
<ul>
  <li> On the display there are three labels on the left aranged vertically and one label on the right. 
  <p> These labales correspond to the three switches on the left and the rotary encoder on the right. From top to down it goes: "[Feed]", "[Play]" and "[Sleep]". Then on the right: "[Bath]".  </p> </li>
  <li> At the bottom of the display there are the pet's current stats in long rectangular bars, which fill up in percentage according to the current value of said stat. It goes from top to bottom in the order: "HUN" (HUNGER), "HAP" (HAPPINESS), "ENG" (ENERGY), "HYG" (HYGIENE). </li>
  <li> The screen also features an age variable which keeps track of how many minutes your pet has been alive. </li>
  </ul>
  
<h3> BUTTONS </h3>
<ul>
  <li>The first button triggers the feed screen. The leds blinks and the buzzer rings in morse code to form the message: "EAT"
    <p>On this screen pressing the same button increases the hunger and energy stat and makes the pet more full. When the hunger stat reaches 100 the tamagotchi plays a little jingle</p> 
  </li>
  
  <li> The second button triggers the play screen. The leds blinks and the buzzer rings in morse code to form the message: "YAY"
     <p>On this screen pressing the same button increases the happiness stat but decreases the energy stat making the pet feel more tired. When the happiness reaches 100 the tamagotchi plays a little jingle</p> </li>

  <li> The third button triggers the sleep screen. The leds blinks and the buzzer rings in morse code to form the message: "ZZZ"
  <p>On this screen pressing the same button increases the energy stat and makes the pet feel more energetic. When the energy reaches 100 the tamagotchi plays a little jingle </p>
  </li>
  
  <li> The fourth button triggers the bath screen. The leds blinks and the buzzer rings in morse code to form the message: "EWW" 
    <p> On this screen you need to turn the rotary encoder a certain amount of time before the hygiene and energy stat increases. The screen displays how many turn are left to increment the stats by the set value. Each turn is equal to 5 rotations. Rotating the encoder a combined amount of 15 time to either the left and or right increases these stats imitaiting a scrubbing motion. Pressing the rotary encoder again returns back to the main screen. When the hygiene stat reaches 100 the tamagotchi plays a little jingle</p> </li>  
</ul>

<h3> MODES </h3>
<ul>
  <li> If the energy stat is below 40 the pet is tired and displays this image: <br>
    <img width="308" height="308" alt="sprite_sleeping" src="https://github.com/user-attachments/assets/9ee537c4-166f-4cb7-b729-df0886a2c571" />
 </li>
  <li> If the hygiene is below 30 the pet is dirty and displays this image: <br>
    <img width="308" height="308" alt="sprite_dirty" src="https://github.com/user-attachments/assets/fb0b593a-d900-4b9f-b008-e7e28358338c" />
 </li>
  <li> If the hunger and happiness stats are below 30 the pet is sad and displays this image: <br> 
    <img width="308" height="308" alt="sprite_sad2" src="https://github.com/user-attachments/assets/12484d0b-871a-4310-8312-c94e559ad050" />
</li>
  <li> If the energy and happiness stats are above 60 the pet is happy and displays this image: <br> 
    <img width="256" height="256" alt="sprite_happy" src="https://github.com/user-attachments/assets/0d166388-52e7-4e30-8592-96173610f264" />
</li>  
<li>
  If the above conditions for the modes aren't met the pet has a neutral expression:  <br>
  <img width="308" height="308" alt="sprite_neutral" src="https://github.com/user-attachments/assets/35ff7b77-9018-41c3-a90d-6e300175473a" />
</li>


### Disclaimer
Since I haven't build it in real life yet there are bound to be inaccuracies in the code or hardware. However I've tried my best to minimize those as much as possible. This image is thus just a vague rendition of what the screen could theoretically look like with the current code. <br>
<img width="509" height="671" alt="main_screen_neutral" src="https://github.com/user-attachments/assets/ad6cee7c-7711-42d7-9e71-18f590c35f93" />

# Firmware setup
Make sure you have the neccesary libraries downloaded in Arduino ide. Select the board and configure the pins to match your pin numbers. At this step you can also customize the ringotchi. <br>
<img width="524" height="513" alt="pin_libraries" src="https://github.com/user-attachments/assets/110b1c54-9bf8-435d-9b7a-fdc227033c96" /> <br>
You can change the tunes that play by changing the notes in the (something_)jingle functions. <br>
<img width="274" height="586" alt="change_jingle" src="https://github.com/user-attachments/assets/3bcd025b-bad6-460b-b88f-aa1582654d8e" /> <br>
And also change the pattern of the morse code to be other messages in the morse(_something) variable at the top. <br>
<img width="404" height="106" alt="change_morse" src="https://github.com/user-attachments/assets/5ef3c9f3-4bc5-40a3-a0c0-88f307f3bae5" /> <br>
Furthermore you can even customize your sprite by drawing it in [https://draw-to-bit.vercel.app/], copying the progmem(c) and replacing it in the code for PROGMEM pet(something). <br> 
<img width="437" height="563" alt="change_sprite" src="https://github.com/user-attachments/assets/12fc68a0-a4da-429e-b71d-b6fd0b5cc2e9" /> <br>

However make sure it is a 32 x 32 bit sprite as those are the dimensions used in this code. After customizing to your hearts desire upload the code to the Xiao ESPC3 and put together the casing.
 

# Casing
<a href='https://cad.onshape.com/documents/550f27ac1857586e625d99ee/w/67f007c674e2b675580248ba/e/c8e1a93a35cedfa3546ef407?renderMode=0&uiState=6a361b78c68ee69fbb8f3a63'>
Onshape link </a> <br> Or copy paste in browser: [https://cad.onshape.com/documents/550f27ac1857586e625d99ee/w/67f007c674e2b675580248ba/e/c8e1a93a35cedfa3546ef407?renderMode=0&uiState=6a361b78c68ee69fbb8f3a63] <br>

<h4>Step 1 :</h4> Building the casing is pretty straight forward.You print out the top and bottom case and gather the nessecary screws.
<h4> Step 1.5 (Optional): </h4> Paint and coat the case in your preffered colors. I recommend colors similar to the 3d model.
<h4> Step 2: </h4> Place the LiPo battery in the bottom compartment of the bottom case and connect it to the headers on the pcb.

<img width="989" height="714" alt="case_noComponets" src="https://github.com/user-attachments/assets/6787472c-f55d-4808-b605-e3bf212a650f" />

<h4> Step 3:</h4> Screw the pcb in place using two M3 screws on either sides of the pcb.
<img width="1131" height="797" alt="ringotchi_case_open" src="https://github.com/user-attachments/assets/105f5585-f017-4ff4-8415-421de6b8ea82" />

<h4>Step 4:</h4> Attach the top case by screwing it in place using 6 M3 screws.
<img width="1054" height="690" alt="ringotchi_case_side" src="https://github.com/user-attachments/assets/7e76ed87-fffc-4664-9c59-3dc34d0e6d4f" />

Your Ringotchi is now fully assembled!
<img width="987" height="782" alt="ringotchi_case_face" src="https://github.com/user-attachments/assets/d792e640-4c6a-484b-bf6e-e4566b2778ff" />









# PCB

<h2> Schematics</h2>
<img width="1362" height="838" alt="schematics_ringotchi" src="https://github.com/user-attachments/assets/e634e5e9-fdbc-4ccd-9802-fa0592e623ed" />

<h2> 3d Model PCB</h2>
<img width="1372" height="924" alt="pcb3d_front_ringotchi" src="https://github.com/user-attachments/assets/47fedb9f-618e-4088-a330-db5d84cc7569" />
<img width="1500" height="940" alt="pcb3d_back_ringotchi" src="https://github.com/user-attachments/assets/33d0ba86-e592-4ea2-80ba-c99d8e369221" />

<h2>PCB Routing </h2>
<img width="980" height="771" alt="pcbRoute_ringotchi" src="https://github.com/user-attachments/assets/38652f9e-abac-4d36-8e5b-329d10dbd592" />



# BOM (Bill of Materials)
[Ringotchi BOM - Sheet1.csv](https://github.com/user-attachments/files/29273973/Ringotchi.BOM.-.Sheet1.csv)

| Item                                                                                               | Description                                 | Quantity | Unit Price ($) | Total Price($) | URL                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        | Running Total With Tax ($) |
|----------------------------------------------------------------------------------------------------|---------------------------------------------|----------|----------------|----------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------------------------|
| M3 screws (50 pcs)                                                                                 | 3mm diameter counter sunk screws 8mm (6pcs) | 1        | 0.1296         | 6.48           | https://nl.aliexpress.com/item/1005008709978631.html?spm=a2g0o.detail.pcDetailTopMoreOtherSeller.2.454fftMBftMBpy&gps-id=pcDetailTopMoreOtherSeller&scm=1007.40050.354490.0&scm_id=1007.40050.354490.0&scm-url=1007.40050.354490.0&pvid=83f03170-c03f-4f9e-a1b3-7860960ec42c&_t=gps-id%3ApcDetailTopMoreOtherSeller%2Cscm-url%3A1007.40050.354490.0%2Cpvid%3A83f03170-c03f-4f9e-a1b3-7860960ec42c%2Ctpp_buckets%3A668%232846%238114%231999&pdp_ext_f=%7B%22order%22%3A%2211679%22%2C%22eval%22%3A%221%22%2C%22sceneId%22%3A%2230050%22%2C%22fromPage%22%3A%22recommend%22%7D&pdp_npi=6%40dis%21EUR%211.96%211.80%21%21%2114.93%2113.73%21%400b884b5817817984658936804e0fbf%2112000046342342830%21rec%21NL%213328622363%21XZ%211%210%21n_tag%3A-29919%3Bd%3A9b918edc%3Bm03_new_user%3A-29895%3BpisId%3A5000000209393528&utparam-url=scene%3ApcDetailTopMoreOtherSeller%7Cquery_from%3A%7Cx_object_id%3A1005008709978631%7C_p_origin_prod%3A | 6.48                       |
| Lithium Polymer Battery 3.7 V 500 mAh 403048 Lipo Rechargeable Battery with Wired JST Connector    | LiPo Battery                                | 1        | 11.6           | 11.6           | https://www.amazon.nl/gp/product/B095W6742D/ref=ox_sc_act_title_1?smid=ASLRAS85RTYN3&th=1                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  | 11.6                       |
| 12*9.5mm Mini Active Piezo Buzzers (10 pcs)                                                        | 12x9.5 Buzzer 1pc                           | 1        | 0.694          | 6.94           | https://nl.aliexpress.com/item/1005010321957502.html?gatewayAdapt=usa2nld4itemAdapt                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        | 1                          |
| ESP32 C3 Development Board for Arduino Seeed Studio XIAO ESP32C3                                   | MCU                                         | 1        | 6.26           | 6.26           | https://nl.aliexpress.com/item/1005006979844970.html?spm=a2g0o.productlist.main.1.37f9760dY6UkfV&algo_pvid=e796eff8-5341-4eeb-94a3-04cde44d927c&algo_exp_id=e796eff8-5341-4eeb-94a3-04cde44d927c-0&pdp_ext_f=%7B%22order%22%3A%22506%22%2C%22spu_best_type%22%3A%22price%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%2111.93%215.49%21%21%2190.17%2141.49%21%40211b6c1717822614569675506eeb93%2112000051688055643%21sea%21NL%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3A9b918edc%3Bm03_new_user%3A-29895&curPageLogUid=BNs0zElQh3h5&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005006979844970%7C_p_origin_prod%3A                                                                                                                                                                                                                                                                   | 6.26                       |
| 0.96 inch OLED display module SSD1306 I2C SPI serial 128X64 LCD 4-pin white (Pin Headers soldered) | OLED Display                                | 1        | 2.5            | 2.5            | https://nl.aliexpress.com/item/1005006996785298.html?spm=a2g0o.productlist.main.7.3fa6sXSnsXSn6B&algo_pvid=0320fd94-73fb-46c3-b134-a735887beb62&algo_exp_id=0320fd94-73fb-46c3-b134-a735887beb62-6&pdp_ext_f=%7B%22order%22%3A%2233299%22%2C%22eval%22%3A%221%22%2C%22orig_sl_item_id%22%3A%221005006996785298%22%2C%22orig_item_id%22%3A%221005007389730469%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%214.40%210.89%21%21%2133.25%216.73%21%40211b441e17822617235672929e65bb%2112000038989536071%21sea%21NL%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3A9b918edc%3Bm03_new_user%3A-29895%3BpisId%3A5000000205243325&curPageLogUid=Dw1SKvcomtXI&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005006996785298%7C_p_origin_prod%3A1005007389730469                                                                                                                                                             | 2.5                        |
| 5 pcs Encoder Switch EC11 With Push Button Switch 5-pin Plug-in 15mm Half Plump Shaft              | Rotary Encoder with push button 1pc         | 1        | 0.648          | 3.24           | https://nl.aliexpress.com/item/1005009120433387.html?spm=a2g0o.detail.pcDetailTopMoreOtherSeller.13.2f7feUDoeUDoei&gps-id=pcDetailTopMoreOtherSeller&scm=1007.40050.354490.0&scm_id=1007.40050.354490.0&scm-url=1007.40050.354490.0&pvid=6460b935-0420-4cc0-84cc-4b231cf40f24&_t=gps-id%3ApcDetailTopMoreOtherSeller%2Cscm-url%3A1007.40050.354490.0%2Cpvid%3A6460b935-0420-4cc0-84cc-4b231cf40f24%2Ctpp_buckets%3A668%232846%238111%231996&pdp_ext_f=%7B%22order%22%3A%22471%22%2C%22eval%22%3A%221%22%2C%22sceneId%22%3A%2230050%22%2C%22fromPage%22%3A%22recommend%22%7D&pdp_npi=6%40dis%21EUR%213.56%212.85%21%21%213.97%213.18%21%400b88ab7917822626711124499e10dc%2112000047989804997%21rec%21NL%21%21ABXZ%211%210%21n_tag%3A-29910%3Bd%3A9b918edc%3Bm03_new_user%3A-29895&utparam-url=scene%3ApcDetailTopMoreOtherSeller%7Cquery_from%3A%7Cx_object_id%3A1005009120433387%7C_p_origin_prod%3A                                       | 3.24                       |
| Tactile Switch 6x6x10mm Center pin 2 pins (20 pcs)                                                 | 6mm Tactile switches 3pcs                   | 1        | 0.312          | 6.24           | https://nl.aliexpress.com/item/1005006120482626.html?spm=a2g0o.productlist.main.1.40354ccbylfe9E&algo_pvid=6e561830-6e62-4b52-ad51-589a1727a57a&algo_exp_id=6e561830-6e62-4b52-ad51-589a1727a57a-0&pdp_ext_f=%7B%22order%22%3A%22475%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%215.41%213.62%21%21%2140.90%2127.37%21%402103864c17822638743823953ee021%2112000035862032475%21sea%21NL%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3A9b918edc%3Bm03_new_user%3A-29895%3BpisId%3A5000000205243325&curPageLogUid=iZrToCtnBZvV&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005006120482626%7C_p_origin_prod%3A                                                                                                                                                                                                                                                                             | 6.24                       |
| 50 Pieces 2.54mm Single Row Pin Header Connector Strip 2Pin                                        | 2pin Pin Headers                            | 1        | 0.107          | 5.35           | https://nl.aliexpress.com/item/4000988113226.html?spm=a2g0o.cart.0.0.53c261d7OkfEao&mp=1&pdp_npi=6%40dis%21EUR%21EUR+1.79%21EUR+1.79%21%21EUR+1.79%21%21%21%402161390417815650514934912e10d2%2110000013202368857%21ct%21NL%213328622363%21%211%210%21&gatewayAdapt=glo2nld                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 | 5.35                       |
| 20 pieces metal film resistor 470K ohm                                                             | 470k ohm resistor                           | 1        | 0.4595         | 9.19           | https://nl.aliexpress.com/item/1005011814571889.html?spm=a2g0o.productlist.main.3.1b7c6511xsp3Yq&algo_pvid=1d3a1db1-713d-4efb-9de3-b824852553d3&algo_exp_id=1d3a1db1-713d-4efb-9de3-b824852553d3-2&pdp_ext_f=%7B%22order%22%3A%22123%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%2116.77%218.03%21%21%21126.77%2160.68%21%40211b61bb17822653509184226ea90a%2112000056662713535%21sea%21NL%213328622363%21X%211%210%21n_tag%3A-29919%3Bd%3A9b918edc%3Bm03_new_user%3A-29895%3BpisId%3A5000000209850550&curPageLogUid=ArSjgp03ZoD7&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005011814571889%7C_p_origin_prod%3A                                                                                                                                                                                                                                                                    | 9.19                       |
| 100 pcs 3mm LED Diode RGB                                                                          | 2 LEDs                                      | 1        | 0.0106         | 1.06           | https://nl.aliexpress.com/item/1005007989424003.html?spm=a2g0o.productlist.main.4.7ca62085FxEFKQ&aem_p4p_detail=202606231847252829649531052840004049485&algo_pvid=fe6b06b1-495e-4a56-9680-0fddfabc552d&algo_exp_id=fe6b06b1-495e-4a56-9680-0fddfabc552d-3&pdp_ext_f=%7B%22order%22%3A%22551%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%210.99%210.88%21%21%211.11%210.99%21%402103894417822656452717554e1bfb%2112000043174317952%21sea%21NL%213328622363%21X%211%210%21n_tag%3A-29919%3Bd%3A9b918edc%3Bm03_new_user%3A-29895&curPageLogUid=Hn4y39cmrsXJ&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005007989424003%7C_p_origin_prod%3A&search_p4p_id=202606231847252829649531052840004049485_1                                                                                                                                                                                    | 1.06                       |


