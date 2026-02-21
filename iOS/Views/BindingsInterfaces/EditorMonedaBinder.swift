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
    
    /// Binds the UI elements of the given controller to their respective data sources.
    /// - Parameter controller: The controller containing the UI elements to bind.
    func bind(controller: EditorMonedaController)
    
    func unbind()
}

