// PIN pour le sens du moteur 1
int moteur1_1 = 2;
int moteur1_2 = 4;
// PIN pour la puissance du moteur 1
int moteur1_p = 3;

// PIN pour le sens du moteur 2
int moteur2_1 = 5;
int moteur2_2 = 7;
// PIN pour la puissance du moteur 2
int moteur2_p = 6;

// sens
int avant = 1;
int arriere = -1;
int arret = 0;

// moteurs
int moteur1 = 1;
int moteur2 = 2;

void setup() {
  pinMode(moteur1_1,OUTPUT);
  pinMode(moteur1_2,OUTPUT);
  pinMode(moteur1_p,OUTPUT);
  pinMode(moteur2_1,OUTPUT);
  pinMode(moteur2_2,OUTPUT);
  pinMode(moteur2_p,OUTPUT);
}

void loop() {
  /*for (int i = 0; i<=100; i+=10) {
    actionMoteur(moteur1, avant,i);
    delay(500);
  }*/
  
  
  actionMoteur(moteur1, avant, 100);
  delay(2000);
  
  actionMoteur(moteur1, arret, 0);
  actionMoteur(moteur2, avant, 100);
  
  delay(2000);
  
  actionMoteur(moteur1, arriere, 50);
  actionMoteur(moteur2, arriere, 0);
  
  delay(2000);
  
  actionMoteur(moteur1, arret, 0);
  actionMoteur(moteur2, arriere, 50);
  
  delay(2000);
  
  actionMoteur(moteur1, arret, 0);
  actionMoteur(moteur2, arret, 0);
  
  delay(1000);
}

void actionMoteur(int moteur,int sens,int pPuissance) {
  int pin1, etat1, pin2, etat2, pinP, puissance;
  
  //test numéro du moteur
  if (moteur == moteur1){
    pin1=moteur1_1;
    pin2=moteur1_2;
    pinP=moteur1_p;
  } else if (moteur == moteur2) {
    pin1=moteur2_1;
    pin2=moteur2_2;
    pinP=moteur2_p;
  } else {
    return;
  }
  
  //test sens du moteur 1,-1 (sens contrainre) ou tout autre valeur (stoppe le moteur)
  if (sens == avant){
    etat1 = 1;
    etat2 = 0;
  } else if (sens == arriere) {
    etat1 = 0;
    etat2 = 1;
  } else {
    etat1 = 0;
    etat2 = 0;
  }
  
  puissance = map(pPuissance, 0, 100, 0, 255);
  
  analogWrite(pinP, puissance);
  digitalWrite(pin1, etat1);
  digitalWrite(pin2, etat2);

}
