/*
 * ========================================
 * ESP8266 - MQTT con Broker Público EMQX
 * Compatible con GitHub Pages
 * ========================================
 * 
 * Broker: broker.emqx.io
 * Puerto TCP: 1883
 * Tópico: /regalo/mensaje
 * WebSocket (página web): ws://broker.emqx.io:8083/mqtt
 * 
 * Librerías necesarias:
 * - ESP8266WiFi (incluida)
 * - PubSubClient (instalar desde Library Manager)
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// ========================================
// CONFIGURACIÓN WiFi - EDITAR AQUÍ
// ========================================
const char* ssid = "TU_WIFI_SSID";          // 🔧 Cambia por tu WiFi
const char* password = "TU_WIFI_PASSWORD";   // 🔧 Cambia por tu contraseña

// ========================================
// CONFIGURACIÓN MQTT - EDITAR AQUÍ
// ========================================
const char* mqtt_server = "broker.emqx.io";  // Broker público EMQX
const int mqtt_port = 1883;                   // Puerto TCP (NO WebSocket)
const char* mqtt_topic = "/regalo/mensaje";   // Tópico (mismo que la web)
const char* mqtt_client_id = "esp8266_regalo"; // ID único

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
  Serial.println("========================================");
  Serial.print("Conectando a WiFi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("✅ WiFi conectado");
    Serial.print("📡 IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("📶 RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println();
    Serial.println("❌ Error: No se pudo conectar a WiFi");
  }
}

// ========================================
// CALLBACK: Mensaje MQTT recibido
// ========================================
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("========================================");
  Serial.print("📨 Mensaje recibido en [");
  Serial.print(topic);
  Serial.print("]: ");
  
  // Convertir payload a String
  String mensaje = "";
  for (int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }
  Serial.println(mensaje);

  // ========================================
  // 🎯 AQUÍ PROCESA EL MENSAJE RECIBIDO
  // ========================================
  
  // Ejemplo 1: Control del LED integrado
  if (mensaje == "ON" || mensaje == "on" || mensaje == "1") {
    digitalWrite(LED_BUILTIN, LOW);  // LED encendido (LOW en ESP8266)
    Serial.println("💡 LED encendido");
  } 
  else if (mensaje == "OFF" || mensaje == "off" || mensaje == "0") {
    digitalWrite(LED_BUILTIN, HIGH); // LED apagado
    Serial.println("💡 LED apagado");
  }
  else if (mensaje == "BLINK" || mensaje == "blink") {
    Serial.println("💡 LED parpadeando...");
    for (int i = 0; i < 6; i++) {
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
    }
  }
  else {
    // Mensaje personalizado
    Serial.print("📝 Mensaje personalizado: ");
    Serial.println(mensaje);
    
    // 🔧 AGREGA AQUÍ TU LÓGICA PERSONALIZADA
    // Ejemplos:
    // - Control de servo
    // - Actualizar LCD
    // - Cambiar color RGB
    // - Activar relé
    // - Reproducir sonido
    // - etc.
  }
  
  Serial.println("========================================");
}

// ========================================
// FUNCIÓN: Reconectar MQTT
// ========================================
boolean reconnect() {
  Serial.print("⏳ Intentando conectar a MQTT... ");
  
  if (client.connect(mqtt_client_id)) {
    Serial.println("✅ Conectado");
    Serial.print("📍 Broker: ");
    Serial.print(mqtt_server);
    Serial.print(":");
    Serial.println(mqtt_port);
    Serial.print("📌 Tópico: ");
    Serial.println(mqtt_topic);
    
    // Suscribirse al tópico
    client.subscribe(mqtt_topic);
    Serial.println("✅ Suscrito al tópico");
    
    // Publicar mensaje de confirmación (opcional)
    String mensaje = "ESP8266 conectado desde " + WiFi.localIP().toString();
    client.publish(mqtt_topic, mensaje.c_str());
    
    return true;
  } else {
    Serial.print("❌ Fallo. Estado: ");
    Serial.println(client.state());
    return false;
  }
}

// ========================================
// SETUP INICIAL
// ========================================
void setup() {
  // Inicializar Serial
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n\n");
  Serial.println("========================================");
  Serial.println("🎁 ESP8266 - MQTT Control Remoto");
  Serial.println("========================================");
  Serial.print("🌐 Broker: ");
  Serial.print(mqtt_server);
  Serial.print(":");
  Serial.println(mqtt_port);
  Serial.print("📍 Tópico: ");
  Serial.println(mqtt_topic);
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
  
  lastReconnectAttempt = 0;
  
  Serial.println();
  Serial.println("🚀 Sistema iniciado correctamente");
  Serial.println("========================================");
}

// ========================================
// LOOP PRINCIPAL
// ========================================
void loop() {
  // Verificar conexión WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi desconectado. Reconectando...");
    setup_wifi();
  }
  
  // Verificar y mantener conexión MQTT
  if (!client.connected()) {
    unsigned long now = millis();
    
    // Reintentar cada 5 segundos
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      
      if (reconnect()) {
        lastReconnectAttempt = 0;
      } else {
        Serial.println("⏭️ Reintentando en 5 segundos...");
      }
    }
  } else {
    // Procesar mensajes MQTT
    client.loop();
  }
  
  // Pequeña pausa
  delay(10);
}

/*
 * ========================================
 * 📋 INSTRUCCIONES DE USO
 * ========================================
 * 
 * 1. CONFIGURAR WIFI:
 *    - Edita las líneas 21-22 con tu red WiFi
 * 
 * 2. SUBIR AL ESP8266:
 *    - Instala librería "PubSubClient" (Tools → Manage Libraries)
 *    - Selecciona tu placa ESP8266
 *    - Sube el código
 * 
 * 3. MONITOREAR:
 *    - Abre Monitor Serial (115200 baud)
 *    - Verás "✅ Conectado" cuando esté listo
 * 
 * 4. ENVIAR MENSAJES:
 *    - Desde la página web: https://tu-usuario.github.io/cajita/mqtt-sender.html
 *    - O desde cualquier cliente MQTT apuntando a:
 *      Broker: broker.emqx.io:1883
 *      Tópico: /regalo/mensaje
 * 
 * ========================================
 * 🧪 MENSAJES DE PRUEBA
 * ========================================
 * 
 * Desde la página web, prueba enviar:
 * - "ON"    → Enciende el LED
 * - "OFF"   → Apaga el LED
 * - "BLINK" → Parpadea 3 veces
 * - Cualquier texto personalizado
 * 
 * ========================================
 * 🔧 ESTADOS DEL CLIENT
 * ========================================
 * 
 * -4 : MQTT_CONNECTION_TIMEOUT
 * -3 : MQTT_CONNECTION_LOST
 * -2 : MQTT_CONNECT_FAILED
 * -1 : MQTT_DISCONNECTED
 *  0 : MQTT_CONNECTED ✅
 *  1 : MQTT_CONNECT_BAD_PROTOCOL
 *  2 : MQTT_CONNECT_BAD_CLIENT_ID
 *  3 : MQTT_CONNECT_UNAVAILABLE
 *  4 : MQTT_CONNECT_BAD_CREDENTIALS
 *  5 : MQTT_CONNECT_UNAUTHORIZED
 * 
 * ========================================
 * ⚠️ NOTAS IMPORTANTES
 * ========================================
 * 
 * 1. BROKER PÚBLICO:
 *    - broker.emqx.io es gratuito y sin autenticación
 *    - Los mensajes son públicos
 *    - Usa un tópico único para tu proyecto
 * 
 * 2. COMPATIBILIDAD:
 *    - Compatible con la página web en GitHub Pages
 *    - Ambos usan el mismo broker y tópico
 *    - La web usa WebSocket (8083), el ESP usa TCP (1883)
 * 
 * 3. PERSONALIZACIÓN:
 *    - Modifica la función callback() para tu lógica
 *    - Agrega control de servos, LCD, RGB, etc.
 *    - Mantén el mismo tópico en web y ESP8266
 * 
 * ========================================
 */
