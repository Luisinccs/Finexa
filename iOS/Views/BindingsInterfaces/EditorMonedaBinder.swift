//
//  EditorMonedaBinder.swift
//  FinexaViewsIos
//
//  Created by Luis D'Andrea on 15/2/26.
//

import UIKit

// MARK: - Binder Protocol

/// Protocol for binding EditorMoneda UI elements to data logic.
/// This allows the Controller to be independent of specific ViewModel types.
public protocol EditorMonedaBinder: FormBindingProtocol {
    
    /// Binds the text fields to their respective data sources.
    /// - Parameters:
    ///   - nombre: Etiqueta para el nombre.
    ///   - simbolo: Etiqueta para el nombre.
    ///   - siglas: Etiqueta para el nombre.
    func bindFields(nombre: UITextField, simbolo: UITextField, siglas: UITextField)
    
    /// Vincula las etiquetas de los campos del editor con el ViewModel.
    /// - Parameters:
    ///   - lblNombre: Etiqueta para el nombre.
    ///   - lblSimbolo: Etiqueta para el nombre.
    ///   - lblSiglas: Etiqueta para el nombre.
    func bindLabels(lblNombre: UILabel, lblSimbolo: UILabel, lblSiglas: UILabel)
    
    
    func bindDialog(to viewController: UIViewController)
    func unbind()
}

