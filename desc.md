- Contrôleur ESP32-wroom sur une carte de développement de 38 broches
- Un contrôleur de moteur TMC2209 de BIGTREETECH ( Je ne pense pas utiliser l'UART )
- 1 écran LCD I2C de 2 lignes de 16 caractères
- 1 Step moteur NEMA17 17HS4401S
- 1 Joystick KY-023
- 1 bouton de navigation à 5 canaux 
-2 Limit switch ( NC ou NO )
- Je possède des résistances et condensateurs en tout genre au besoin
- Je possèdes les alimentations électriques nécessaires au projet.




Excellente idée ! Avec un outil comme Fritzing ou KiCad, tu pourras créer un schéma clair et personnalisé pour ton projet. Voici quelques conseils pour te guider :

1. Plan des connexions principales
  - ESP32 : Place-le au centre, car il est le contrôleur principal.
  - TMC2209 : Connecte les broches STEP, DIR, et EN à des GPIO de l’ESP32.
  - Écran LCD : Branche SDA et SCL sur les broches I2C par défaut (GPIO 21 et GPIO 22).
  - Joystick : Branche VRX, VRY (analogiques) et SW (digital) sur des GPIO libres.
  - Bouton 5 canaux : Connecte chaque sortie à des GPIO disponibles avec résistances pull-down.
  - Interrupteurs fin de course : Branche sur des GPIO avec résistances pull-up internes ou externes.