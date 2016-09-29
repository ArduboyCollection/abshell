#include "globals.h"
#include "lib/core/ntshell.h"
extern "C" {
#include "ntshell_arduino.h"
}
#include "usrcmd_arduino.h"

#define PROMPTSTR "Arduboy>"

static ntshell_t ntshell;

static int func_read(char *buf, int cnt, void *extobj) {
  if (Serial.available())
    return Serial.readBytes(buf, cnt);
  else
    return 0;
}

static int func_write(const char *buf, int cnt, void *extobj) {
  return Serial.write(buf, cnt);
}

static int func_callback(const char *text, void *extobj) {
#if 0
  Serial.print("User input text:'");
  Serial.print(text);
  Serial.print("'\r\n");
#else
  return usrcmd_execute(text);
#endif
}

void setup() {
  arduboy.begin();
  arduboy.clear();
  arduboy.setFrameRate(SCRN_FPS);

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  ntshell_init(
    &ntshell,
    func_read,
    func_write,
    func_callback,
    (void *)(&ntshell));

  ntshell_set_prompt(&ntshell, PROMPTSTR);
  Serial.println("Welcome to Arduboy.\r\n type 'help' for help.");
  Serial.print(PROMPTSTR);
  Serial.flush();
}

void loop() {
  ntshell_execute_arduino(&ntshell);

  if (!(arduboy.nextFrame()))
    return;

  arduboy.display();
}
