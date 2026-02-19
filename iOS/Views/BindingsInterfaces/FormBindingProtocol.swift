import UIKit
import DcViewsIos

/// Protocol that abstracts common form binding operations.
public protocol FormBindingProtocol {
    
    /// Binds the command bar buttons to their respective commands.
    /// - Parameters:
    ///   - bar: The command bar component.
    func bindCommands(bar: DcCommandBar)
    
    /// Binds the close request event from the ViewModel.
    /// - Parameter handler: Closure to execute when close is requested.
    func bindCloseRequest(handler: @escaping () -> Void)
}
