#include <OneWire.h>
#include <DallasTemperature.h>

class TempSensor
{
  private:
    int sensorPin;
    double sensorTemperature;
    double sensorMaxTemperature;
    double sensorMinTemperature;
    double sensorAvgTemperature;
    unsigned long numberOfReadings = 0;
    unsigned long avgSumHelper;
    OneWire* sensorOneWire;
    DallasTemperature* sensor;

  public:


    TempSensor(int pin) {
      this->sensorPin = pin;
      sensorOneWire = new OneWire(this->sensorPin);
      sensor = new DallasTemperature(this->sensorOneWire);
      sensor->begin();
      this->sensorMaxTemperature, this->sensorMinTemperature, this->sensorAvgTemperature, this->sensorMinTemperature, this->sensorMaxTemperature = temp();
    }


    int showPin() {
      return this->sensorPin;
    }

    double updateTemp() {
      sensor->requestTemperatures();
      this->sensorTemperature = sensor->getTempCByIndex(0);

      if (this->sensorTemperature == -127) return -127.0;

      this->numberOfReadings++;
      this->avgSumHelper += sensorTemperature;

      if (this->sensorTemperature < this->sensorMinTemperature) this->sensorMinTemperature = this->sensorTemperature;
      if (this->sensorTemperature > this->sensorMaxTemperature) this->sensorMaxTemperature = this->sensorTemperature;

      return this->sensorTemperature;
    }

    double temp() {
      return this->sensorTemperature;
    }

    double avgTemp() {
      temp();
      this->sensorAvgTemperature = this->avgSumHelper / this->numberOfReadings;
      return this->sensorAvgTemperature;
    }

    String statistics(String format) {
      format.replace("%Max", String(this->sensorMaxTemperature, 0));
      format.replace("%Min", String(this->sensorMinTemperature, 0));
      format.replace("%Avg", String(this->sensorAvgTemperature, 0));
      format.replace("%Cur", String(this->sensorTemperature, 0));
      return  format;
    }

    void resetStatistics() {
      this->sensorMaxTemperature = sensorTemperature;
      this->sensorMinTemperature = sensorTemperature;
      this->sensorAvgTemperature = sensorTemperature;
      this->sensorTemperature = sensorTemperature;
    }


};
