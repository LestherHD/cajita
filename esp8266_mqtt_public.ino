/*
 * ESP8266 - MQTT con Broker Público
 * Compatible con la página web alojada en GitHub Pages
 * 
 * Broker: broker.emqx.io
 * Puerto TCP: 1883
 * Tópico: /regalo/mensaje
 * 
 * Librerías necesarias:
 * - ESP8266WiFi
 * - PubSubClient
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// ========================================
// CONFIGURACIÓN WiFi - EDITAR AQUÍ
// ========================================
const char* ssid = "TU_WIFI_SSID";          // Nombre de tu red WiFi
const char* password = "TU_WIFI_PASSWORD";   // Contraseña de tu WiFi

// ========================================
// CONFIGURACIÓN MQTT - EDITAR AQUÍ
// ========================================
const char* mqtt_server = "broker.emqx.io";  // Broker MQTT público
const int mqtt_port = 1883;                   // Puerto TCP (no WebSocket)
const char* mqtt_topic = "/regalo/mensaje";   // Tópico para recibir mensajes
const char* mqtt_client_id = "esp8266_regalo"; // ID único del cliente

// ========================================
// VARIABLES GLOBALES
// ========================================
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastReconnectAttempt = 0;

// ========================================
// FUNCIÓN: Conectar a WiFi
// ========================================
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a WiFi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("✓ WiFi conectado");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

// ========================================
// CALLBACK: Mensaje MQTT recibido
// ========================================
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido en [");
  Serial.print(topic);
  Serial.print("]: ");
  
  String mensaje = "";
  for (int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }
  Serial.println(mensaje);

  // ========================================
  // AQUÍ PROCESA EL MENSAJE RECIBIDO
  // ========================================
  // Ejemplo: controlar un LED, servo, etc.
  
  if (mensaje == "ON") {
    digitalWrite(LED_BUILTIN, LOW);  // LED encendido (LOW en ESP8266)
    Serial.println("→ LED encendido");
  } 
  else if (mensaje == "OFF") {
    digitalWrite(LED_BUILTIN, HIGH); // LED apagado
    Serial.println("→ LED apagado");
  }
  else {
    Serial.println("→ Mensaje personalizado recibido: " + mensaje);
    // Agrega aquí tu lógica personalizada
  }
}

// ========================================
// FUNCIÓN: Reconectar MQTT
// ========================================
boolean reconnect() {
  if (client.connect(mqtt_client_id)) {
    Serial.println("✓ Conectado al broker MQTT");
    Serial.print("Suscrito al tópico: ");
    Serial.println(mqtt_topic);
    
    // Suscribirse al tópico
    client.subscribe(mqtt_topic);
    
    // Publicar mensaje de confirmación (opcional)
    client.publish(mqtt_topic, "ESP8266 conectado");
    
    return true;
  }
  return false;
}

// ========================================
// SETUP INICIAL
// ========================================
void setup() {
  // Inicializar Serial
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n\n========================================");
  Serial.println("ESP8266 - MQTT con Broker Público");
  Serial.println("========================================");
  
  // Configurar LED integrado
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // Apagado inicialmente
  
  // Conectar a WiFi
  setup_wifi();
  
  // Configurar MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.setKeepAlive(60);
  
  Serial.print("Broker MQTT: ");
  Serial.print(mqtt_server);
  Serial.print(":");
  Serial.println(mqtt_port);
  
  lastReconnectAttempt = 0;
}

// ========================================
// LOOP PRINCIPAL
// ========================================
void loop() {
  // Verificar conexión WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("✗ WiFi desconectado. Reconectando...");
    setup_wifi();
  }
  
  // Verificar y mantener conexión MQTT
  if (!client.connected()) {
    unsigned long now = millis();
    
    // Reintentar cada 5 segundos
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      
      Serial.println("Intentando reconectar a MQTT...");
      
      if (reconnect()) {
        lastReconnectAttempt = 0;
      } else {
        Serial.print("✗ Fallo en conexión MQTT. Estado: ");
        Serial.println(client.state());
        Serial.println("  Reintentando en 5 segundos...");
      }
    }
  } else {
    // Procesar mensajes MQTT
    client.loop();
  }
  
  // Pequeña pausa para no saturar el procesador
  delay(10);
}

/*
 * ========================================
 * NOTAS IMPORTANTES
 * ========================================
 * 
 * 1. BROKER PÚBLICO:
 *    - broker.emqx.io es un broker público gratuito
 *    - NO requiere autenticación
 *    - Los tópicos son compartidos (usa nombres únicos)
 * 
 * 2. PUERTOS:
 *    - ESP8266 usa puerto 1883 (TCP)
 *    - Página web usa puerto 8083 (WebSocket)
 *    - Ambos se comunican a través del mismo broker
 * 
 * 3. SEGURIDAD:
 *    - Al usar un broker público, cualquiera puede ver tus mensajes
 *    - Para producción, considera usar un broker privado
 *    - O implementa autenticación con usuario/contraseña
 * 
 * 4. COMPATIBILIDAD:
 *    - Compatible con la página web en GitHub Pages
 *    - No requiere configuración de red local
 *    - Funciona desde cualquier lugar con Internet
 * 
 * 5. ESTADOS DEL CLIENT:
 *    -4 : MQTT_CONNECTION_TIMEOUT
 *    -3 : MQTT_CONNECTION_LOST
 *    -2 : MQTT_CONNECT_FAILED
 *    -1 : MQTT_DISCONNECTED
 *     0 : MQTT_CONNECTED
 *     1 : MQTT_CONNECT_BAD_PROTOCOL
 *     2 : MQTT_CONNECT_BAD_CLIENT_ID
 *     3 : MQTT_CONNECT_UNAVAILABLE
 *     4 : MQTT_CONNECT_BAD_CREDENTIALS
 *     5 : MQTT_CONNECT_UNAUTHORIZED
 */
