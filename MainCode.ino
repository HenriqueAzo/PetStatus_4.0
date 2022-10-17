#include <Keypad.h>
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <EEPROM.h>


const byte ROWS = 4; //quatro linhas
const byte COLS = 4; //três colunas


 char keys[ROWS][COLS] = {// matriz que inclui as teclas usadas
  {'1','2','3', 'A'},          //
  {'4','5','6', 'B'},          //
  {'7','8','9', 'C'},          //
  {'*','0','#', 'D'}           //
};                        //


byte rowPins[ROWS] = {2, 3, 4, 5}; //pinos usados para ler as linhas do teclado
byte colPins[COLS] = {6, 7, 8, 9}; //pinos usados para ler as colunas do teclado


Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );//determina os parâmetros do teclado

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

//FIM DO SETUP IMUTAVEL

char senha = "";//variável para armazenar a senha a ser inserida pelo usuário
char nova_senha = "";//variável para armazenar a nova senha a ser inserida pelo usuário
char asteriscos = "";




//função para abrir a porta
void abrirporta(){
  digitalWrite(10, HIGH);
  delay(500);
  digitalWrite(10, LOW);
}




void reconhecimento(String senha){//função para verificar se a senha inserida é correta


}
  
void reconhecimento_eeprom(String senha){//função para verificar se a senha inserida é correta
  int usuario = 1;//variável que armazena o usuario
  bool verify = 0;//variável que armazena o reconhecimento da senha

  for(usuario; usuario<20; usuario++){ //Sao limitados a 20 senhas possiveis
    String senha_teste = ""; //Variavel para armazenar as senhas registradas na EEPROM
    for(int i = usuario ;i<=(usuario+6) ;i++){ //Busca os 6 caracteres associados ao usuario
      senha_teste.concat(EEPROM.read(i)); 
    }
    if(senha == senha_teste){ // Se a senha for igual à senha associado ao usuario
    verify = 1;
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Bem Vindo,");
    lcd.setCursor(5, 1);
    lcd.print("Usuario ");
    lcd.print(usuario);
    break; //Termina a busca por senhas compativeis
    }
  }
 
  //definindo se a senha foi reconhecida
  if (verify == 1){
    abrirporta();
  }else{
    Serial.println("Senha não reconhecida.");
  }
}
  
void telaInicial(){
   lcd.init();
   lcd.clear();
   lcd.backlight();
   lcd.setCursor(0, 0); // Set the cursor on the third column and first row.
   lcd.print("PET Status");
   lcd.setCursor(1, 1);
   lcd.print("Iniciando...");
   Serial.println("iniciando o sistema");
   digitalWrite(10, LOW);
}

void telaSenha(){
  lcd.clear();
   lcd.backlight();
   lcd.setCursor(0, 0); // Set the cursor on the third column and first row.
   lcd.print("PET Status");
   lcd.setCursor(1, 1);
   lcd.print("senha: ");
}

void telaUser(){
  lcd.clear();
   lcd.backlight();
   lcd.setCursor(0, 0); // Set the cursor on the third column and first row.
   lcd.print("PET Status");
   lcd.setCursor(1, 1);
   lcd.print("usuario: ");
}

void telaWelcomeUser(){//AINDA TEM QUE SER FEITO
   lcd.backlight();
   lcd.setCursor(0, 0); // Set the cursor on the third column and first row.
   lcd.print("PET Status");
   lcd.setCursor(1, 1);
   lcd.print("senha: ");
   Serial.println("iniciando o sistema");
   digitalWrite(10, LOW);
}

void leitura(){
  char key = keypad.getKey();//cria a variável onde o botão apertado será armazenado

}

void CriaSenha(){

}

void setup() {
  Serial.begin(9600);
  pinMode(10, OUTPUT);//pino de acionamento da porta

  //PINOS USADOS PARA INTERRUPT
  pinmode(2, INPUT_PULLDOWN);
  pinmode(3, INPUT_PULLDOWN);

  telaInicial();

  attachInterrupt(digitalPinToInterrupt(2), ISR, RISING);//Leitura do teclado
  attachInterrupt(digitalPinToInterrupt(3), ISR, RISING);//Criação de senha
   
}




void loop() {
  


bool cadastrar_senha = 0;
if(digitalRead(1)==1){  //**Colocar entrada digital certa ao inves da 1
 cadastrar_senha = 1;
}
int numero_de_usuario = 0;
bool pegar_numero_de_usuario = 1;
int endereco_eeprom = 0;


while(cadastrar_senha == 1){ //Quando for requisitado o cadastro de uma nova senha
    
    senha = "";      

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PET Status");
    lcd.setCursor(1, 1);
    lcd.print("Nova senha: ");

if (key != NO_KEY){//segura o código enquanto o programa não recebe input

if(pegar_numero_de_usuario == 1){ //Primeiro é pego o numero de usuario, que serve para associar uma senha a cada petiano e localiza o espaço de memoria da EEPROM a ser escrito
numero_de_usuario = key;
pegar_numero_de_usuario = 0; //So se deve ser feito no inicio do procedimento de cadastro de nova senha
endereco_eeprom = 6*numero_de_usuario + 1; //Como cada senha tem 6 caracteres
}

else{
  
EEPROM.write(endereco_eeprom,key); //É cadastrado o caractere (numero da senha) no espaço da EEPROM (cada senha ocupará 6 bytes de espaço)
endereco_eeprom++; //Passa-se para o proximo endereço
nova_senha.concat(key);
asteriscos.concat('*'); 
lcd.setCursor(7, 1);
lcd.print(asteriscos); //Aparece um asterisco sempre que um novo caractere é inserido pelo usuario

if (nova_senha.length() == 6){ //Quando for atingido o tamanho da senha o processo é finalizado
    nova_senha = "";
    asteriscos = "";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PET Status");
    lcd.setCursor(1, 1);
    lcd.print("senha: ");
    pegar_numero_de_usuario = 1;
    cadastrar_senha = 0;
}  
} 
}
}
  
if (key != NO_KEY){//segura o código enquanto o programa não recebe input

  char lastkey = key;
  Serial.println("apertado: ");
  Serial.println('*'); //coloca a tecla apertada no monitor serial

  senha.concat(key);//adiciona a tecla pressionada no final da String que é a senha
  asteriscos.concat('*');
  
  if (senha.indexOf("#") != -1){//limpa a senha caso seja inserido um #
      senha = "";
    }
  
  Serial.println("senha: ");
  Serial.println(senha);
  lcd.setCursor(7, 1);
  lcd.print(asteriscos);
  
//reconhece o tamanho da senha e dá início ao reconhecimento
  if (senha.length() == 6){
    reconhecimento(senha);
    senha = "";
    asteriscos = "";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PET Status");
    lcd.setCursor(1, 1);
    lcd.print("senha: ");
  }

}
}//FIM DO VOID LOOP
