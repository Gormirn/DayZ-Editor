


class Controller: Managed
{
	// Private members
	private ref PropertyHashMap m_ControllerPropertyHashMap;
	private ref array<ref Controller> m_ChildControllers = {};
	void AddChildController(Controller controller) {
		m_ChildControllers.Insert(controller);
	}
	
	
	// Protected members
	protected Widget m_LayoutRoot;
	Widget GetLayoutRoot() {
		return m_LayoutRoot;
	}
	
	protected ref ViewBindingHashMap m_ViewBindingHashMap = new ViewBindingHashMap();
	protected ref PropertyHashMap m_PropertyHashMap;
	typename GetPropertyType(string property_name) {
		return m_PropertyHashMap.Get(property_name);
	}
	
	void Controller() { EditorLog.Trace("Controller"); }
	void ~Controller() { EditorLog.Trace("~Controller"); }
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("Controller::Init");
		m_LayoutRoot = w;
				
		// User must inherit from controller, not use it in ScriptClass
		if (Type() == Controller) {
			EditorLog.Error("You cannot bind to data without creating your own controller class!");
			return;
		}
		
		// Load all properties of the inheriting Controller
		m_PropertyHashMap = PropertyHashMap.FromType(Type());
		
		// Gets rid of properties that only exist in this class
		//PropertyHashMap controller_hashbrowns = PropertyHashMap.FromType(Controller);
		// Commented due to crashes for whatever reason smile :)
		//foreach (string name, typename type: controller_hashbrowns) {
		//	m_PropertyHashMap.Remove(name);
		//}
		
		EditorLog.Info(string.Format("%1 Properties found!", m_PropertyHashMap.Count()));
		
		// Load all child Widgets and obtain their DataBinding class
		int binding_count = LoadDataBindings(m_LayoutRoot, m_ViewBindingHashMap);
		if (binding_count == 0) {
			EditorLog.Warning("No DataBindings found! Is the controller in a parent Widget?");
		} else {
			EditorLog.Info(string.Format("%1 DataBindings found!", binding_count));
		}
		
		
		// debug
		m_ViewBindingHashMap.DebugPrint();
		/*
		foreach (string data_name, DataBindingBase data: m_DataBindingHashMap) {
			PropertyInfo prop = m_PropertyHashMap.GetPropertyInfo(data_name);
			if (!data.CanConvertFrom(prop.Type)) {
				ErrorDialog(string.Format("Invalid data type in %1. Found %2, supports %3", data_name, prop.Type, data.GetType()));
				m_DataBindingHashMap.Remove(data_name);
			}
		}*/
	}
	
	// This must be called manually by the user
	void NotifyPropertyChanged(string property_name)
	{
		EditorLog.Trace("Controller::NotifyPropertyChanged " + property_name);
		ref ViewBindingSet view_set = m_ViewBindingHashMap.Get(property_name);
		
		if (view_set) {
			foreach (ViewBinding view: view_set) {
				view.OnPropertyChanged();
			}
		}
		
		PropertyChanged(property_name);
	}
	
	// This gets called automatically when a collection is changed
	void NotifyCollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		EditorLog.Trace("Controller::NotifyCollectionChanged " + collection_name);
		ref ViewBindingSet view_set = m_ViewBindingHashMap.Get(collection_name);
		
		if (view_set) {
			foreach (ViewBinding view: view_set) {
				view.OnCollectionChanged(args);
			}
		}
		
		CollectionChanged(collection_name, args);
	}
	
	// Called every time a property is changed. 
	// Override this when you want to have an event AFTER property is changed
	void PropertyChanged(string property_name);
	
	// Called every time an observable collection is changed.
	// Override this when you want to have an event AFTER collection is changed
	void CollectionChanged(string collection_name, CollectionChangedEventArgs args);
	
	
	
	private int LoadDataBindings(Widget w, out ViewBindingHashMap binding_map)
	{
		ViewBinding view_binding;
		w.GetScript(view_binding);
		
		if (view_binding && (view_binding.IsInherited(ViewBinding))) {
			binding_map.InsertView(view_binding.GetBindingName(), view_binding);
			view_binding.SetController(this);
		}
		
		if (w.GetChildren() != null)
			LoadDataBindings(w.GetChildren(), binding_map);
		
		
		if (w.GetSibling() != null) 
			LoadDataBindings(w.GetSibling(), binding_map);
		
		
		return binding_map.Count();
	}
	

	
	void OnMouseDown(Widget w, int button, int x, int y)
		foreach (Controller c: m_ChildControllers)
			c.OnMouseDown(w, button, x, y);
		
	void OnMouseUp(Widget w, int button, int x, int y)
		foreach (Controller c: m_ChildControllers)
			c.OnMouseUp(w, button, x, y);
	
	void OnMouseWheel(Widget w, int direction, int x, int y)
		foreach (Controller c: m_ChildControllers)
			c.OnMouseWheel(w, direction, x, y);
	
	void OnClick(Widget w, int button, int x, int y)
		foreach (Controller c: m_ChildControllers)
			c.OnClick(w, button, x, y);
	
	void OnDoubleClick(Widget w, int button, int x, int y)
		foreach (Controller c: m_ChildControllers)
			c.OnDoubleClick(w, button, x, y);
	
	void OnKeyPress(int key)
		foreach (Controller c: m_ChildControllers)
			c.OnKeyPress(key);
	
	void OnMouseEnter(Widget w, int x, int y)
		foreach (Controller c: m_ChildControllers)
			c.OnMouseEnter(w, x, y);
	
	void OnMouseLeave(Widget w, Widget enter_w, int x, int y)
		foreach (Controller c: m_ChildControllers)
			c.OnMouseLeave(w, enter_w, x, y);
	
	void OnDrag(Widget target, int x, int y)
		foreach (Controller c: m_ChildControllers)
			c.OnDrag(target, x, y);
	
	void OnDrop(Widget target, Widget drop_target, int x, int y)
		foreach (Controller c: m_ChildControllers)
			c.OnDrop(target, drop_target, x, y);
	
	void OnDragging(Widget target, int x, int y)
		foreach (Controller c: m_ChildControllers)
			c.OnDragging(target, x, y);
	
	void OnDropReceived(Widget target, Widget received_target, int x, int y)
		foreach (Controller c: m_ChildControllers)
			c.OnDropReceived(target, received_target, x, y);
}











