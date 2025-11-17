# 🎁 Cajita - Control MQTT con ESP8266

Sistema de mensajería MQTT entre una página web (GitHub Pages) y un ESP8266 usando el broker público **broker.emqx.io**.

## 📋 Descripción

Este proyecto permite enviar mensajes desde una página web responsive a un ESP8266 a través de MQTT, sin necesidad de configurar un broker propio.

## 🌐 Arquitectura

```
┌─────────────────────┐                           ┌──────────────────┐
│   Página Web HTML   │   WebSocket (8083/mqtt)   │                  │
│   (GitHub Pages)    │ ─────────────────────────▶│  broker.emqx.io  │
│                     │                            │   Broker Público │
└─────────────────────┘                            │                  │
                                                   │                  │
┌─────────────────────┐      TCP (1883)           │                  │
│      ESP8266        │ ◀─────────────────────────│                  │
│  (Arduino/NodeMCU)  │                            └──────────────────┘
└─────────────────────┘

        Tópico compartido: /regalo/mensaje
```

## ⚙️ Configuración

### 📄 Página Web (`mqtt-sender.html`)

```javascript
const BROKER_HOST = "broker.emqx.io";
const BROKER_PORT = 8083;
const MQTT_TOPIC = "/regalo/mensaje";
const WS_URL = "ws://broker.emqx.io:8083/mqtt";
```

### 🔌 ESP8266 (`esp8266_cajita.ino`)

```cpp
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_topic = "/regalo/mensaje";
```

## 🚀 Instrucciones de Uso

### 1️⃣ Configurar la Página Web

✅ **Ya está lista para usar**. Solo necesitas:

1. Subir `mqtt-sender.html` a tu repositorio GitHub
2. Activar GitHub Pages (Settings → Pages → Main branch)
3. Acceder a: `https://tu-usuario.github.io/cajita/mqtt-sender.html`

### 2️⃣ Configurar el ESP8266

1. **Instalar librerías** en Arduino IDE:
   - `ESP8266WiFi` (incluida con soporte ESP8266)
   - `PubSubClient` (Tools → Manage Libraries → buscar "PubSubClient")

2. **Editar credenciales WiFi** en `esp8266_cajita.ino`:
   ```cpp
   const char* ssid = "TU_WIFI_SSID";
   const char* password = "TU_WIFI_PASSWORD";
   ```

3. **Subir el código** al ESP8266

4. **Abrir Monitor Serial** (115200 baud)

### 3️⃣ Probar la Conexión

1. El ESP8266 mostrará: `✅ Conectado` y `✅ Suscrito al tópico`
2. Abre la página web
3. Espera el indicador verde "Conectado"
4. Escribe un mensaje y presiona "Enviar Mensaje"
5. El ESP8266 recibirá el mensaje (visible en Monitor Serial)

## 🧪 Mensajes de Prueba

Desde la página web, prueba enviar:

| Mensaje | Acción en ESP8266 |
|---------|-------------------|
| `ON` | Enciende LED integrado |
| `OFF` | Apaga LED integrado |
| `BLINK` | Parpadea 3 veces |
| Cualquier texto | Se muestra en Serial |

## 📡 Detalles Técnicos

### Puertos y Protocolos

| Componente | Protocolo | Puerto | URL |
|------------|-----------|--------|-----|
| Página Web | WebSocket | 8083 | `ws://broker.emqx.io:8083/mqtt` |
| ESP8266 | TCP | 1883 | `broker.emqx.io:1883` |

### Broker MQTT

- **Host**: broker.emqx.io
- **Tipo**: Público y gratuito
- **Autenticación**: No requerida
- **Ruta WebSocket**: `/mqtt` (obligatoria)

## 📝 Personalización

### Cambiar el Tópico

Para usar un tópico único (recomendado):

**En `mqtt-sender.html`:**

```javascript
const MQTT_TOPIC = "/regalo/micajita123/mensaje";
```

**En `esp8266_cajita.ino`:**

```cpp
const char* mqtt_topic = "/regalo/micajita123/mensaje";
```

### Modificar Comportamiento del ESP8266

Edita la función `callback()` para agregar tu lógica personalizada:

```cpp
void callback(char* topic, byte* payload, unsigned int length) {
  String mensaje = "";
  for (int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }
  
  // Tu código aquí
  if (mensaje == "SERVO_90") {
    // Mover servo a 90°
  } else if (mensaje == "LCD_HOLA") {
    // Mostrar en LCD
  }
  // etc.
}
```

## 🎨 Cambiar Estilo de la Página

Edita el CSS en `mqtt-sender.html`:

```css
/* Cambiar el degradado de fondo */
background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);

/* Cambiar color del botón */
.btn-send {
  background: linear-gradient(135deg, #FF6B6B 0%, #FF8E53 100%);
}
```

## 🐛 Solución de Problemas

### ❌ Página no conecta al broker

**Solución:**
1. Abre la consola del navegador (F12)
2. Busca errores de conexión
3. Verifica que uses: `ws://broker.emqx.io:8083/mqtt`
4. Prueba desde: https://www.hivemq.com/demos/websocket-client/

### ❌ ESP8266 no conecta

**Verifica en Monitor Serial:**

```
✅ WiFi conectado
✅ Conectado a MQTT
✅ Suscrito al tópico
```

**Si no aparece:**
- Revisa credenciales WiFi
- Verifica que el broker sea accesible
- Comprueba el puerto (1883)

### ❌ Mensajes no llegan

**Checklist:**
- ✅ Mismo broker: `broker.emqx.io`
- ✅ Mismo tópico: `/regalo/mensaje`
- ✅ Ambos conectados (verde en web, "Conectado" en Serial)
- ✅ El tópico es case-sensitive (`/Regalo/Mensaje` ≠ `/regalo/mensaje`)

## 📚 Librerías Utilizadas

### Página Web
- **Paho MQTT JavaScript** (v1.0.1)
  - URL: https://www.eclipse.org/paho/index.php?page=clients/js/index.php
  - CDN: https://cdnjs.cloudflare.com/ajax/libs/paho-mqtt/1.0.1/mqttws31.min.js

### ESP8266
- **PubSubClient** (Nick O'Leary)
  - GitHub: https://github.com/knolleary/pubsubclient
  - Instalación: Arduino IDE → Tools → Manage Libraries → "PubSubClient"

## 🔗 Enlaces Útiles

- [Broker EMQX](https://www.emqx.io/)
- [GitHub Pages](https://pages.github.com/)
- [ESP8266 Arduino Core](https://github.com/esp8266/Arduino)
- [MQTT.org](https://mqtt.org/)

## 📄 Archivos del Proyecto

```
cajita/
├── mqtt-sender.html          # Página web para enviar mensajes
├── esp8266_cajita.ino        # Código Arduino para ESP8266
├── esp8266_mqtt_public.ino   # Versión alternativa del código
├── README.md                 # Descripción del repositorio
└── README_MQTT.md            # Este archivo (documentación completa)
```

## ⚡ Quick Start

1. **Clonar repositorio:**
   ```bash
   git clone https://github.com/LestherHD/cajita.git
   cd cajita
   ```

2. **Configurar ESP8266:**
   - Abre `esp8266_cajita.ino` en Arduino IDE
   - Cambia WiFi SSID y contraseña
   - Sube al ESP8266

3. **Publicar página:**
   - Ve a GitHub → Settings → Pages
   - Activa GitHub Pages en rama `main`
   - Accede a tu URL

4. **¡Listo!** 🎉
   - Abre la página web
   - Envía mensajes
   - Ve los resultados en el Monitor Serial

## 📄 Licencia

MIT License - Código abierto y libre para usar

## 👨‍💻 Autor

**LestherHD**
- GitHub: [@LestherHD](https://github.com/LestherHD)
- Proyecto: [cajita](https://github.com/LestherHD/cajita)

---

### ⭐ ¿Te gustó el proyecto?

Si este proyecto te fue útil, considera darle una estrella ⭐ en GitHub

---

**Última actualización:** Noviembre 2025

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
