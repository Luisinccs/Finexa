/**
 * @file FinexaWebBindings.hpp
 * @author Finexa Team
 * @date 2026-01-31
 * @brief Finexa Application Bindings for Emscripten
 * 
 * This file contains the Emscripten binding registrations for the Finexa
 * application including Core entities and ViewModels.
 */

#ifndef FINEXA_WEB_BINDINGS_CORE_HPP
#define FINEXA_WEB_BINDINGS_CORE_HPP

#ifndef __EMSCRIPTEN__
#error "This file is only for Emscripten/WebAssembly builds"
#endif

#include <emscripten.h>
#include <emscripten/bind.h>

// Include the core framework bindings
#include "WebBindingsCore.hpp"

// Include Finexa Core entities
#include "../../../Core/include/Moneda.h"
#include "../../../Core/include/TasaDeCambio.h"
#include "../../../Core/include/Operacion.h"
#include "../../../Core/include/CalculadoraCore.h"

// Include Finexa ViewModels
#include "../../../ViewModels/include/FinexaMainViewModel.h"
#include "../../../ViewModels/include/OperacionesViewModel.h"
#include "../../../ViewModels/include/TasasViewModel.h"

/**
 * @brief Registers all Finexa application-specific classes with Emscripten
 * 
 * Call this macro inside your EMSCRIPTEN_BINDINGS() block, usually after
 * calling DC_VIEWMODELS_REGISTER_INTERFACES().
 * 
 * USAGE:
 *   EMSCRIPTEN_BINDINGS(FinexaApp) {
 *     DC_VIEWMODELS_REGISTER_INTERFACES();
 *     FINEXA_REGISTER_APP_BINDINGS();
 *   }
 */
#define FINEXA_REGISTER_APP_BINDINGS() \
    /* Register Core Entities */ \
    emscripten::class_<Finexa::Moneda>("Moneda") \
        .smart_ptr<std::shared_ptr<Finexa::Moneda>>("MonedaPtr") \
        .function("getId", &Finexa::Moneda::getId) \
        .function("getSiglas", &Finexa::Moneda::getSiglas) \
        .function("getNombre", &Finexa::Moneda::getNombre) \
        .function("getSimbolo", &Finexa::Moneda::getSimbolo); \
    \
    emscripten::class_<Finexa::TasaDeCambio>("TasaDeCambio") \
        .smart_ptr<std::shared_ptr<Finexa::TasaDeCambio>>("TasaDeCambioPtr") \
        .function("getBase", &Finexa::TasaDeCambio::getBase) \
        .function("getDestino", &Finexa::TasaDeCambio::getDestino) \
        .function("getValor", &Finexa::TasaDeCambio::getValor) \
        .function("setValor", &Finexa::TasaDeCambio::setValor) \
        .function("getParidad", &Finexa::TasaDeCambio::getParidad); \
    \
    emscripten::class_<Finexa::Operacion>("Operacion") \
        .smart_ptr<std::shared_ptr<Finexa::Operacion>>("OperacionPtr") \
        .function("getConcepto", &Finexa::Operacion::getConcepto) \
        .function("getMontoOriginal", &Finexa::Operacion::getMontoOriginal) \
        .function("getMonedaOriginal", &Finexa::Operacion::getMonedaOriginal) \
        .function("getMontoXds", &Finexa::Operacion::getMontoXds) \
        .function("setConcepto", &Finexa::Operacion::setConcepto) \
        .function("setMontoOriginal", &Finexa::Operacion::setMontoOriginal) \
        .function("setMonedaOriginal", &Finexa::Operacion::setMonedaOriginal) \
        .function("setMontoXds", &Finexa::Operacion::setMontoXds); \
    \
    emscripten::class_<Finexa::CalculadoraCore>("CalculadoraCore") \
        .smart_ptr<std::shared_ptr<Finexa::CalculadoraCore>>("CalculadoraCorePtr") \
        .constructor<>() \
        .function("buscarMoneda", &Finexa::CalculadoraCore::buscarMoneda) \
        .function("registrarMoneda", &Finexa::CalculadoraCore::registrarMoneda) \
        .function("buscarTasa", &Finexa::CalculadoraCore::buscarTasa) \
        .function("establecerTasa", &Finexa::CalculadoraCore::establecerTasa) \
        .function("calcularValorImplicito", &Finexa::CalculadoraCore::calcularValorImplicito) \
        .function("convertirAXds", &Finexa::CalculadoraCore::convertirAXds) \
        .function("tieneAmbiguedad", &Finexa::CalculadoraCore::tieneAmbiguedad) \
        .function("getSiglasPivote", &Finexa::CalculadoraCore::getSiglasPivote) \
        .function("getSiglasObjetivo", &Finexa::CalculadoraCore::getSiglasObjetivo) \
        .function("getMonedas", &Finexa::CalculadoraCore::getMonedas) \
        .function("getTasas", &Finexa::CalculadoraCore::getTasas); \
    \
    /* Register ViewModels */ \
    emscripten::class_<Finexa::ViewModels::OperacionesViewModel>("OperacionesViewModel") \
        .smart_ptr<std::shared_ptr<Finexa::ViewModels::OperacionesViewModel>>("OperacionesViewModelPtr") \
        .class_function("create", emscripten::select_overload<std::shared_ptr<Finexa::ViewModels::OperacionesViewModel>()>(&Finexa::ViewModels::OperacionesViewModel::create)) \
        .function("gridOperaciones", &Finexa::ViewModels::OperacionesViewModel::gridOperacionesBinding, emscripten::allow_raw_pointers()) \
        .function("inputConcepto", &Finexa::ViewModels::OperacionesViewModel::inputConceptoBinding, emscripten::allow_raw_pointers()) \
        .function("inputMonto", &Finexa::ViewModels::OperacionesViewModel::inputMontoBinding, emscripten::allow_raw_pointers()) \
        .function("selectorMoneda", &Finexa::ViewModels::OperacionesViewModel::selectorMonedaBinding, emscripten::allow_raw_pointers()) \
        .function("cmdAgregar", &Finexa::ViewModels::OperacionesViewModel::cmdAgregarBinding, emscripten::allow_raw_pointers()) \
        .function("inicializar", &Finexa::ViewModels::OperacionesViewModel::inicializar) \
        .function("agregarOperacion", &Finexa::ViewModels::OperacionesViewModel::agregarOperacion) \
        .function("refrescarGrilla", &Finexa::ViewModels::OperacionesViewModel::refrescarGrilla); \
    \
    emscripten::class_<Finexa::ViewModels::TasasViewModel>("TasasViewModel") \
        .smart_ptr<std::shared_ptr<Finexa::ViewModels::TasasViewModel>>("TasasViewModelPtr") \
        .class_function("create", emscripten::select_overload<std::shared_ptr<Finexa::ViewModels::TasasViewModel>()>(&Finexa::ViewModels::TasasViewModel::create)) \
        .function("gridTasas", &Finexa::ViewModels::TasasViewModel::gridTasasBinding, emscripten::allow_raw_pointers()) \
        .function("selectorBase", &Finexa::ViewModels::TasasViewModel::selectorBaseBinding, emscripten::allow_raw_pointers()) \
        .function("selectorDestino", &Finexa::ViewModels::TasasViewModel::selectorDestinoBinding, emscripten::allow_raw_pointers()) \
        .function("inputValor", &Finexa::ViewModels::TasasViewModel::inputValorBinding, emscripten::allow_raw_pointers()) \
        .function("cmdGuardarTasa", &Finexa::ViewModels::TasasViewModel::cmdGuardarTasaBinding, emscripten::allow_raw_pointers()) \
        .function("inicializar", &Finexa::ViewModels::TasasViewModel::inicializar) \
        .function("guardarTasa", &Finexa::ViewModels::TasasViewModel::guardarTasa) \
        .function("refrescarGrilla", &Finexa::ViewModels::TasasViewModel::refrescarGrilla); \
    \
    emscripten::class_<Finexa::ViewModels::FinexaMainViewModel>("FinexaMainViewModel") \
        .smart_ptr<std::shared_ptr<Finexa::ViewModels::FinexaMainViewModel>>("FinexaMainViewModelPtr") \
        .class_function("create", emscripten::select_overload<std::shared_ptr<Finexa::ViewModels::FinexaMainViewModel>()>(&Finexa::ViewModels::FinexaMainViewModel::create)) \
        .function("getOperacionesVm", &Finexa::ViewModels::FinexaMainViewModel::getOperacionesVm) \
        .function("getTasasVm", &Finexa::ViewModels::FinexaMainViewModel::getTasasVm) \
        .function("inicializar", &Finexa::ViewModels::FinexaMainViewModel::inicializar); \
    \
    /* Register vectors if needed */ \
    emscripten::register_vector<std::shared_ptr<Finexa::Moneda>>("VectorMonedaPtr"); \
    emscripten::register_vector<std::shared_ptr<Finexa::TasaDeCambio>>("VectorTasaDeCambioPtr"); \
    emscripten::register_vector<std::shared_ptr<Finexa::Operacion>>("VectorOperacionPtr");

#endif // FINEXA_WEB_BINDINGS_CORE_HPP
