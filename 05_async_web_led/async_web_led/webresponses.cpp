#include <Arduino.h>
#include "webresponses.h"

String HttpResponses(bool is404) {
  String str;
  if (is404)
    str = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nConnection: close\n\n";
  else
    str = "HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: keep-alive\n\n";
  return str;
}

String  htmlTop = "<!DOCTYPE html> \
    <html> \
      <head> \
      <title>XIAO ESP32C3 Web Server Example</title> \
      <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> \
      <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /> \
      <link rel=\"icon\" href=\"data:,\"> \
      <style> \
      html{font-family: Helvetica; display:inline-block; margin: 0px auto; text-align: center;} \
      h1{color: #0F3376; padding: 2vh;} \
      p{font-size: 1.5rem;} \
      .button{display: inline-block; background-color: blue; border: none; border-radius: 6px; \
      color: white; font-size: 1.5rem; width: 5em; height: 3em; text-decoration: none; margin: 2px; cursor: pointer;} \
      .button2{background-color: green;} \
      </style> \
      </head> \
      <body> \
      <h1>XIAO ESP32C3 Web Server Example</h1>";

  String htmlBottom = "</body></html>";

String HttpPage(bool is404, bool Status) {
  // The web server will serve either an index.html page or a 404 error page.
  // Both pages share common elements
  String html;
  if (is404)
    html = "<h1 style=\"color: red\">404 Error</h1><p><a href=\"/\"><button class=\"button button2\">Return</button></a></p>";
  else {
    html = "<p>LED: <strong>";
    html += (Status) ? "ON" : "OFF";
    html += "</strong></p><p><a href=\"/?led=toggle\"><button class=\"button\">Toggle</button></a></p>";
  }
  return htmlTop + html + htmlBottom;
}
