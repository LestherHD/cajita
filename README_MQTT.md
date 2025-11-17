# 🎁 Cajita - Control MQTT con ESP8266

Sistema de mensajería MQTT entre una página web (GitHub Pages) y un ESP8266 usando un broker público.

## 📋 Descripción

Este proyecto permite enviar mensajes desde una página web responsive a un ESP8266 a través de MQTT, usando el broker público `broker.emqx.io`.

## 🌐 Componentes

### 1. Página Web (GitHub Pages)
- **Archivo**: `mqtt-sender.html`
- **Protocolo**: MQTT sobre WebSockets
- **Puerto**: 8083
- **Características**:
  - Diseño responsive y moderno
  - Conexión automática al broker
  - Reconexión automática
  - Notificaciones visuales
  - Validación de mensajes

### 2. ESP8266 (Arduino)
- **Archivo**: `esp8266_mqtt_public.ino`
- **Protocolo**: MQTT TCP
- **Puerto**: 1883
- **Características**:
  - Conexión automática a WiFi
  - Reconexión automática a MQTT
  - Recepción de mensajes
  - Ejemplo con LED integrado

## ⚙️ Configuración

### Página Web

Edita las siguientes variables en `mqtt-sender.html`:

```javascript
const BROKER_HOST = "broker.emqx.io";      // Broker MQTT
const BROKER_PORT = 8083;                   // Puerto WebSocket
const MQTT_TOPIC = "/regalo/mensaje";       // Tópico
const USE_TLS = false;                      // WebSocket seguro
```

### ESP8266

Edita las siguientes variables en `esp8266_mqtt_public.ino`:

```cpp
// WiFi
const char* ssid = "TU_WIFI_SSID";
const char* password = "TU_WIFI_PASSWORD";

// MQTT
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_topic = "/regalo/mensaje";
```

## 🚀 Uso

### 1. Configurar ESP8266

1. Instala las librerías necesarias en Arduino IDE:
   - `ESP8266WiFi` (incluida con el soporte de ESP8266)
   - `PubSubClient` (por Nick O'Leary)

2. Edita las credenciales WiFi en el código

3. Sube el código al ESP8266

4. Abre el Monitor Serial (115200 baud) para ver los logs

### 2. Publicar Página Web en GitHub Pages

1. Sube el archivo `mqtt-sender.html` a tu repositorio

2. Ve a Settings → Pages

3. Selecciona la rama `main` y guarda

4. Accede a tu página: `https://tu-usuario.github.io/cajita/mqtt-sender.html`

### 3. Enviar Mensajes

1. Abre la página web en cualquier navegador

2. Espera a que aparezca "Conectado" (indicador verde)

3. Escribe tu mensaje y haz clic en "Enviar Mensaje"

4. El ESP8266 recibirá el mensaje y lo procesará

## 📡 Arquitectura

```
┌─────────────────┐         WebSocket (8083)         ┌──────────────────┐
│   Página Web    │ ─────────────────────────────────▶│                  │
│  (GitHub Pages) │                                    │  broker.emqx.io  │
└─────────────────┘                                    │   (Broker MQTT)  │
                                                       │                  │
┌─────────────────┐            TCP (1883)             │                  │
│    ESP8266      │ ◀─────────────────────────────────│                  │
│   (Arduino)     │                                    └──────────────────┘
└─────────────────┘

Tópico compartido: /regalo/mensaje
```

## 🔧 Puertos y Protocolos

| Componente | Protocolo | Puerto | Uso |
|------------|-----------|--------|-----|
| Página Web | WebSocket | 8083 | Publicar mensajes |
| ESP8266 | TCP | 1883 | Recibir mensajes |
| Broker | MQTT | Ambos | Intermediario |

## ⚠️ Notas Importantes

1. **Broker Público**: `broker.emqx.io` es gratuito y no requiere autenticación, pero los mensajes son públicos.

2. **Tópico Único**: Usa un tópico único para tu proyecto (ej: `/regalo/usuario123/mensaje`)

3. **Seguridad**: Para producción, considera usar un broker privado con autenticación.

4. **Internet Requerido**: Ambos dispositivos necesitan conexión a Internet.

5. **GitHub Pages**: Solo funciona con brokers públicos accesibles vía WebSocket.

## 🎨 Personalización

### Cambiar Colores

Edita el CSS en `mqtt-sender.html`:

```css
background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
```

### Modificar Comportamiento ESP8266

Edita la función `callback()` en el archivo `.ino`:

```cpp
void callback(char* topic, byte* payload, unsigned int length) {
  // Tu código aquí
}
```

## 📚 Librerías Utilizadas

- **Paho MQTT JavaScript**: Cliente MQTT para navegadores
- **PubSubClient**: Cliente MQTT para Arduino/ESP8266

## 🐛 Solución de Problemas

### Página no conecta

- Verifica que el broker esté accesible: `broker.emqx.io`
- Comprueba el puerto WebSocket: `8083`
- Revisa la consola del navegador (F12)

### ESP8266 no conecta

- Verifica las credenciales WiFi
- Comprueba el Monitor Serial (115200 baud)
- Asegúrate de que el broker sea accesible desde tu red

### Mensajes no llegan

- Verifica que ambos usen el mismo tópico
- Comprueba que ambos estén conectados al broker
- Revisa que el tópico sea correcto (case-sensitive)

## 📄 Licencia

Este proyecto es de código abierto y está disponible bajo la licencia MIT.

## 👨‍💻 Autor

LestherHD

## 🔗 Enlaces Útiles

- [Broker EMQX](https://www.emqx.io/)
- [PubSubClient Library](https://github.com/knolleary/pubsubclient)
- [Paho MQTT JavaScript](https://www.eclipse.org/paho/index.php?page=clients/js/index.php)
- [GitHub Pages](https://pages.github.com/)
