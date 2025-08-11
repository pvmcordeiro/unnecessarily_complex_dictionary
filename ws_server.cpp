#include "worker/WorkerControler.hpp"
#include "commons/UCDLogger.hpp"

int main() {
    try {
        UCDLogger::getInstance()->enableStdoutPrint(true);
        UCD_LOGGER(LOG_INFO, "Starting application");

        WorkerControler controler;

        controler.startConnectionThread();
    
    } catch (const std::exception& e) {
        UCD_LOGGER(LOG_CRIT, "Fatal error: " + std::string(e.what()));
    }
    return 0;
}