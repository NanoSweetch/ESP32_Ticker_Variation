This code was originally written by Patrick-E-Rankin. 

<h3><u>Libraries Used:</u></h3>
    <p><a href="https://github.com/bartoszbielawski/LEDMatrixDriver" target="_blank">LEDMatrixDriver</a>
      by <a href="https://github.com/bartoszbielawski" target="_blank">Bartosz
        Bielawski</a><br>
      <a href="https://github.com/me-no-dev/ESPAsyncWebServer" target="_blank">ESPAsyncWebServer</a>
      by <a href="https://github.com/me-no-dev" target="_blank">Me No Dev</a><br>
  <a href="https://github.com/bblanchon/ArduinoJson">ArduinoJson</a></p>
    <h3><u>Setup</u></h3>
    <p>The ESP32 setup is as follows:</p>

   <table>
<thead>
  <tr>
    <th>ESP32 Pins</th>
    <th>LED Matrix</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td>3.3V</td>
    <td>VCC</td>
  </tr>
  <tr>
    <td>GND</td>
    <td>GND</td>
  </tr>
  <tr>
    <td>23</td>
    <td>DIN</td>
  </tr>
  <tr>
    <td>15</td>
    <td>CS</td>
  </tr>
  <tr>
    <td>18</td>
    <td>CLK</td>
  </tr>
</tbody>
</table>

<p>You must insert your own SSID and password for your Wifi, so it can
      connect. </p>
    <p>Upon getting an IP address it will display it continually on the LED
      Matrix until it receives its first ticker symbol.</p>
    <p>Use any browser to go to the IP address of the ESP32 where you will be presented with a
      field where you can enter the ticker you want on your MAX7219. </p>
    <p>The program will automatically check for a new price every 5 seconds.</p>
    <p>It also supports crypto, but you must use the Yahoo Finance format, for
      example BTC-USD for Bitcoin.</p>
