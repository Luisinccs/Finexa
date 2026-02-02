import Cocoa

class AppDelegate: NSObject, NSApplicationDelegate {
    
    var window: NSWindow!
    
    func applicationDidFinishLaunching(_ aNotification: Notification) {
        
        _ = NSViewController()
        
        window = NSWindow(
            contentRect: NSRect(x: 0, y: 0, width: 1100, height: 600),
            styleMask: [.titled, .closable, .miniaturizable, .resizable],
            backing: .buffered, defer: false)
        
        window.center()
        window.title = "Finexa"
        window.minSize = NSSize(width: 1100, height: 600) // Force minimum size
        
        // Configuración moderna de Toolbar/Titlebar
        window.titlebarAppearsTransparent = true
        window.styleMask.insert(.fullSizeContentView)
        
        // Asignar el SplitView como controlador raíz
        window.contentViewController = TasasController()
        
        // Asegurar que el tamaño se mantenga tras asignar el controlador
        window.setContentSize(NSSize(width: 1100, height: 600))
        
        window.makeKeyAndOrderFront(nil)
    }
    
    func applicationWillTerminate(_ aNotification: Notification) {}
    
    func applicationSupportsSecureRestorableState(_ app: NSApplication) -> Bool {
        return true
    }
    
}
