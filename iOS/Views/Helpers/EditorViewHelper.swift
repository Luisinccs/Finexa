//
//  EditorViewHelper.swift
//  FinexaViewsIos
//

import UIKit

/// Protocol that provides common UI helper methods for Editor Controllers.
public protocol EditorViewHelper {
    /// Creates a vertical stack containing a label and its associated input field.
    /// - Parameters:
    ///   - label: The UILabel to display above the field.
    ///   - field: The input field view (e.g., UITextField, DcComboBox).
    /// - Returns: A UIStackView containing both views arranged vertically.
    func makeFieldStack(label: UILabel, field: UIView) -> UIStackView
}

public extension EditorViewHelper where Self: UIViewController {
    
    func makeFieldStack(label: UILabel, field: UIView) -> UIStackView {
        label.font = .systemFont(ofSize: 14, weight: .semibold)
        label.textColor = .label
        
        // Ensure field has a consistent height if it's not self-sizing
        // For text fields and combo boxes, 44 is standard touch target height
        if field.constraints.filter({ $0.firstAttribute == .height }).isEmpty {
            field.heightAnchor.constraint(equalToConstant: 44).isActive = true
        }
        
        let stack = UIStackView(arrangedSubviews: [label, field])
        stack.axis = .vertical
        stack.spacing = 5
        return stack
    }
}
