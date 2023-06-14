#include <EEPROM.h>

//verificar antes de gravar
int senha[6] = {0, 5, 0, 9, 1, 9}; // Cria um array de 5 inteiros com valores diferentes
int user = 0;
void setup() {
  Serial.begin(9600);
  

  for(int i = 0; i <6; i++) {
    EEPROM.write(i+(user*6), senha[i]);
    Serial.print("Escrevendo o dígito ");
    Serial.print(senha[i]);
    Serial.print(" no endereço ");
    Serial.println(i+(user*6));
  }
}

void loop() {

}
