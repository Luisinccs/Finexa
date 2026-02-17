/**
 * @file Moneda.h
 * @author Finexa Team
 * @date 2026-01-13
 * @brief Entidad que representa una divisa en el sistema core.
 */

#ifndef FINEXA_MONEDA_H
#define FINEXA_MONEDA_H

#include <string>

namespace Finexa {
    /**
     * @class Moneda
     * @brief Clase que encapsula los datos de una moneda con ID unico.
     */
    class Moneda {
    private:
        int id;
        std::string uuid;
        std::string siglas;
        std::string nombre;
        std::string simbolo;

    public:
        Moneda(int id, std::string siglas, std::string nombre, std::string simbolo);
        Moneda(std::string uuid, int id, std::string siglas, std::string nombre, std::string simbolo);

        int getId() const;
        std::string getUuid() const;
        void setUuid(const std::string& newUuid);
        std::string getSiglas() const;
        std::string getNombre() const;
        std::string getSimbolo() const;
    };
}

#endif
