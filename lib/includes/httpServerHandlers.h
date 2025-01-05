
/* WEBERVER TO REFACTOR */

/* Declarations */
void handleFavicon(void);
void handleHTTPRequests(void);
void handleUpload(void);
void handleOnNotFound(void);
void handleDebug(void); 

/* Definistions */

void handleFavicon(void) {}
void handleHTTPRequests(void) { HttpServer->generate(); }
void handleUpload(void) { HttpServer->generate(true); }
void handleOnNotFound(void) {
  String page = "<head><meta http-equiv=\"refresh\" content=\"0; "
                "url=http://192.168.5.1/\" /></head><body><p>Opening "
                "configuration site ...</p></body>";
  HttpServer->publishHTML(page);
}
void handleDebug(void) {
  String page = "";

  //page += Voltage.data.percent;
  page += "<br>";
  //page += Voltage.data.voltage;

  HttpServer->publishHTML(page);
}
