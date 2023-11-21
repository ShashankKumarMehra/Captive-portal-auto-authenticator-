# Captive-portal-auto-authenticator-
Program written in C language running on ESP32. Which uses HTTPClient library to auto authenticate captive portal and maintains the link by passing HTTP request to the server by passing URL query every 3 seconds.

Working of program

CORE0
esp32 connects to wifi > Send HTTP request to the captive portal server with credentials using POST() > LOOP(Send HTTP request to the captive portal server using GET() every 3 seconds)

CORE1
LOOP(Send HTTP request to the internet(www.google.com) using GET() to receive acknowledgement that internet connection is established)
I ran this code on different core only for debuging and some load distribution.
