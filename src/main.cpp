#include <Arduino.h>

/**
 * @brief Pin connected to the boot button LED.
 */
constexpr int LED_BOOT_BUTTON_PIN = 0;

/**
 * @brief PWM channel used for the buzzer.
 */
constexpr int BUZZER_CHANNEL = 0;

/**
 * @brief Output pin connected to the buzzer.
 */
constexpr int BUZZER_PIN = 14;

/**
 * @brief PWM duty cycle used to drive the buzzer.
 */
constexpr int BUZZER_DUTY = 2048;

/**
 * @brief Duration of a dot in milliseconds.
 */
constexpr unsigned int DOT_DURATION_MS = 200;

/**
 * @brief Duration of a dash in milliseconds.
 */
constexpr unsigned int DASH_DURATION_MS = 600;

/**
 * @brief Pause between Morse symbols in milliseconds.
 */
constexpr unsigned int SYMBOL_PAUSE_MS = 600;

/**
 * @brief Pause between words in milliseconds.
 */
constexpr unsigned int WORD_PAUSE_MS = 2000;

/**
 * @brief Pause before repeating the message.
 */
constexpr unsigned int REPEAT_PAUSE_MS = 5000;

/**
 * @brief Returns the Morse code representation of a letter.
 *
 * @param character Uppercase ASCII letter.
 * @return Morse code string for the letter, or nullptr if unsupported.
 */
const char *getMorseForCharacter(char character) {
  switch (character) {
    case 'A': return ".-";
    case 'B': return "-...";
    case 'C': return "-.-.";
    case 'D': return "-..";
    case 'E': return ".";
    case 'F': return "..-.";
    case 'G': return "--.";
    case 'H': return "....";
    case 'I': return "..";
    case 'J': return ".---";
    case 'K': return "-.-";
    case 'L': return ".-..";
    case 'M': return "--";
    case 'N': return "-.";
    case 'O': return "---";
    case 'P': return ".--.";
    case 'Q': return "--.-";
    case 'R': return ".-.";
    case 'S': return "...";
    case 'T': return "-";
    case 'U': return "..-";
    case 'V': return "...-";
    case 'W': return ".--";
    case 'X': return "-..-";
    case 'Y': return "-.--";
    case 'Z': return "--..";
    default:  return nullptr;
  }
}

void flashLight(unsigned int durationMs) {
  digitalWrite(LED_BOOT_BUTTON_PIN, LOW);
  delay(durationMs);
  digitalWrite(LED_BOOT_BUTTON_PIN, HIGH);
}

/**
 * @brief Turns the LED and buzzer on for a specified duration, then off.
 *
 * @param durationMs How long to signal.
 */
void signalMorseElement(unsigned int durationMs) {
  digitalWrite(LED_BOOT_BUTTON_PIN, LOW);
  ledcWrite(BUZZER_CHANNEL, BUZZER_DUTY);

  delay(durationMs);

  ledcWrite(BUZZER_CHANNEL, 0);
  digitalWrite(LED_BOOT_BUTTON_PIN, HIGH);
}

/**
 * @brief Outputs a Morse code sequence using the LED and buzzer.
 *
 * @param morse Null-terminated Morse code sequence containing '.' and '-'.
 */
void outputMorseSequence(const char *morse) {
  while (*morse != '\0') {
    Serial.print(*morse);

    if (*morse == '.') {
      signalMorseElement(DOT_DURATION_MS);
    } else if (*morse == '-') {
      signalMorseElement(DASH_DURATION_MS);
    }

    delay(SYMBOL_PAUSE_MS);
    ++morse;
  }
}

/**
 * @brief Converts a text message to Morse output using the LED and buzzer.
 *
 * Letters are converted to Morse. Spaces and common punctuation are treated
 * as word separators.
 *
 * @param text Input text message.
 */
void playMorseMessage(const String &text) {
  for (unsigned int i = 0; i < text.length(); ++i) {
    char character = toupper(text.charAt(i));

    if (const char *morse = getMorseForCharacter(character)) {
      outputMorseSequence(morse);
      Serial.print(" ");
      delay(SYMBOL_PAUSE_MS);
    } else if (character == ' ' || character == '.' || character == ',' ||
               character == '!' || character == '?') {
      Serial.print("/ ");

      delay(WORD_PAUSE_MS);
    }
  }
}

void playDefaultFlash() {
  flashLight(1000);
  delay(1000);
  flashLight(1000);
  delay(1000);
  flashLight(1500);
}

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(LED_BOOT_BUTTON_PIN, OUTPUT);
  digitalWrite(LED_BOOT_BUTTON_PIN, HIGH);

  ledcSetup(BUZZER_CHANNEL, 25, 12);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  ledcWrite(BUZZER_CHANNEL, 0);

  randomSeed(micros());
}

void loop() {
  playDefaultFlash();
  int randomValue = random(0, 500);
  if(randomValue == 50) {
    playMorseMessage("Hello Caspian.");
  }
  else if(randomValue == 51) {
    playMorseMessage("Your parents love you Caspian.");
  }
  else if(randomValue == 52) {
    playMorseMessage("You were born April second twenty twenty six.");
  }
  else if(randomValue == 53) {
    playMorseMessage("Always stay strong.");
  }
  else if(randomValue == 54) {
    playMorseMessage("We are very proud of you.");
  }
  else if(randomValue == 55) {
    playMorseMessage("Follow your dreams.");
  }
  else if(randomValue == 56) {
    playMorseMessage("We called you Mr Burpy.");
  }
  else if(randomValue == 57) {
    playMorseMessage("You are part Russian, English, Irish, and Lebanese.");
  }
  else if(randomValue == 58) {
    playMorseMessage("There are fourteen hidden messages.");
  }
  else if(randomValue == 59) {
    playMorseMessage("You were born in the Coombe Hospital in Dublin.");
  }
  else if(randomValue == 60) {
    playMorseMessage("You better not throw this lighthouse away.");
  }
  else if(randomValue == 61) {
    playMorseMessage("You are named after the Caspian Sea.");
  }
  else if(randomValue == 62) {
    playMorseMessage("There is a light inside you.");
  }
  else if(randomValue == 63) {
    playMorseMessage("You brighten our lives.");
  }

  // Delay before repeating the message to avoid spamming the LED and buzzer too quickly.
  delay(REPEAT_PAUSE_MS);
  Serial.println();
}