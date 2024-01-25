// STL modules
#include <string>

// ESP modules
#include <nvs_flash.h>
#include <esp_wifi.h>
#include <esp_http_server.h>

// Lightweight IP modules
#include <lwip/sys.h>

// Custom modules
#include "led.hpp"
#include "sensors.hpp"
using namespace kc;

void WifiEventHandler(void* arguments, esp_event_base_t eventBase, int32_t eventId, void* eventData)
{
    switch (eventId)
    {
        case WIFI_EVENT_STA_START:
            Led::Instance->blink({ 255, 255 });
            break;
        case WIFI_EVENT_STA_CONNECTED:
            Led::Instance->glow({ 255, 0 }, 3);
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            Led::Instance->blink({ 255, 255 });
            esp_wifi_connect();
            break;
    }
}

void NetworkInit()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, WifiEventHandler, NULL));
    wifi_init_config_t wifiInitiation = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifiInitiation));

    wifi_config_t wifiConfig = {};
    std::copy(CONFIG_WIFI_NETWORK_SSID, CONFIG_WIFI_NETWORK_SSID + sizeof(CONFIG_WIFI_NETWORK_SSID), reinterpret_cast<char*>(wifiConfig.sta.ssid));
    std::copy(CONFIG_WIFI_NETWORK_PASSWORD, CONFIG_WIFI_NETWORK_PASSWORD + sizeof(CONFIG_WIFI_NETWORK_PASSWORD), reinterpret_cast<char*>(wifiConfig.sta.password));
    ESP_ERROR_CHECK(esp_wifi_set_config(static_cast<wifi_interface_t>(ESP_IF_WIFI_STA), &wifiConfig));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    esp_wifi_connect();
}

std::string MeasurementToJson(const Sensors::Measurement& measurement)
{
    std::string string(256, '\0');
    int stringLength = sprintf(
        string.data(),
        R"({"aht20":{"temperature":%0.2f,"humidity":%0.2f},"bmp280":{"temperature":%0.2f,"pressure":%0.2f}})",
        measurement.aht20.temperature,
        measurement.aht20.humidity,
        measurement.bmp280.temperature,
        measurement.bmp280.pressure
    );

    string.resize(stringLength);
    return string;
}

bool IsAutoRequest(httpd_req_t* request)
{
    std::string query(256, '\0');
    httpd_req_get_url_query_str(request, query.data(), query.size());
    return (query.find("type=auto") != std::string::npos);
}

esp_err_t ErrorHandler(httpd_req_t* request, httpd_err_code_t error)
{
    httpd_resp_send_err(request, HTTPD_404_NOT_FOUND, "Not found. Available resources: /external, /internal");
    return ESP_OK;
}

esp_err_t ExternalResourceHandler(httpd_req_t* request)
{
    std::string jsonString = MeasurementToJson(Sensors::Measure(Sensors::ExternalPort));
    httpd_resp_set_type(request, "application/json");
    httpd_resp_sendstr(request, jsonString.c_str());
    Led::Instance->glow(IsAutoRequest(request) ? Led::Color{ 0, 255 } : Led::Color{ 255, 0 }, 5, 0.3, true);
    return ESP_OK;
}

esp_err_t InternalResourceHandler(httpd_req_t* request)
{
    std::string jsonString = MeasurementToJson(Sensors::Measure(Sensors::InternalPort));
    httpd_resp_set_type(request, "application/json");
    httpd_resp_sendstr(request, jsonString.c_str());
    Led::Instance->glow(IsAutoRequest(request) ? Led::Color{ 0, 255 } : Led::Color{ 255, 0 }, 5, 0.3, true);
    return ESP_OK;
}

void ServerInit()
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    ESP_ERROR_CHECK(httpd_start(&server, &config));
    httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, &ErrorHandler);

    httpd_uri_t externalResource = {};
    externalResource.uri = "/external";
    externalResource.method = HTTP_GET;
    externalResource.handler = &ExternalResourceHandler;
    externalResource.user_ctx = nullptr;
    httpd_register_uri_handler(server, &externalResource);

    httpd_uri_t internalResource = {};
    internalResource.uri = "/internal";
    internalResource.method = HTTP_GET;
    internalResource.handler = &InternalResourceHandler;
    internalResource.user_ctx = nullptr;
    httpd_register_uri_handler(server, &internalResource);
}

extern "C" void app_main(void)
{
    NetworkInit();
    ServerInit();
}
