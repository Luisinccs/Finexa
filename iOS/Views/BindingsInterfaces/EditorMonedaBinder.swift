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
public protocol EditorMonedaBinder {
    /// Binds the text fields to their respective data sources.
    func bind(nombre: UITextField, simbolo: UITextField, siglas: UITextField)
    
    /// Executes the "Accept" action.
    func accept()
    
    /// Executes the "Cancel" action.
    func cancel()
}
