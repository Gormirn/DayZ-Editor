
// on refactor, editor object in constructor
class EditorPlacedListItem: EditorListItem
{
	protected EditorObject m_EditorObject;
	EditorObject GetData() { 
		return m_EditorObject; 
	}
	
	protected ref DragHandler m_DragHandler;
	
	void EditorPlacedListItem(EditorObject editor_object)
	{
		EditorLog.Trace("EditorPlacedListItem::SetEditorObject"); 
		m_EditorObject = editor_object;
		
		m_DragHandler = new DragHandler(m_EditorObject);
		
		m_TemplateController.Label = m_EditorObject.GetDisplayName();
		m_TemplateController.NotifyPropertyChanged("Label");
		
		m_TemplateController.Icon = m_EditorObject.GetData().Icon;
		m_TemplateController.NotifyPropertyChanged("Icon");
				
		m_EditorObject.OnObjectSelected.Insert(EditorObjectSelected);
		m_EditorObject.OnObjectDeselected.Insert(EditorObjectDeselected);	
	}
	
	void ~EditorPlacedListItem()
	{
		delete m_DragHandler;
	}	
	
	void EditorObjectSelected(EditorObject data) 
	{
		Select();
	}
	
	void EditorObjectDeselected(EditorObject data) 
	{
		Deselect();
	}
	
	bool ListItemShowExecute(ButtonCommandArgs args)
	{
		m_EditorObject.Show(!args.GetButtonState());
		return true;
	}
	
	override bool IsSelected() {
		return m_EditorObject.IsSelected();
	}
	
	bool ListItemExecute(ButtonCommandArgs args)
	{
		switch (args.GetMouseButton()) {

			
			case MouseState.LEFT: {

				if (KeyState(KeyCode.KC_LCONTROL)) {
					GetEditor().ToggleSelection(m_EditorObject);
					return true;
				} 
				
				if (!KeyState(KeyCode.KC_LSHIFT)) {
					GetEditor().ClearSelection();
				}
				
				GetEditor().SelectObject(m_EditorObject);
				return true;
			}
			
			case MouseState.MIDDLE: {
				EditorCamera camera = GetEditor().GetCamera();
				vector pos = m_EditorObject.GetPosition();
				pos[1] = camera.GetPosition()[1];
				camera.SetPosition(pos);
				return true;
			}
			
			case MouseState.RIGHT: {
				
				if (!m_EditorObject.IsSelected() && !KeyState(KeyCode.KC_LSHIFT)) {
					GetEditor().ClearSelection();
				}
				
				GetEditor().SelectObject(m_EditorObject);
				
				if (EditorHud.CurrentMenu) {
					delete EditorHud.CurrentMenu;
				}
					
				int x, y;
				GetMousePos(x, y);
				EditorHud.CurrentMenu = new EditorPlacedContextMenu(x, y);
				
				return true;
			}
			
		}
		
		return true;
	}
	
	bool ListItemVisibleExecute(ButtonCommandArgs args)
	{
		switch (args.GetMouseButton()) {
			
			case 0: {
				m_EditorObject.ShowWorldObject(args.GetButtonState());
				break;
			}
			
		}
		
		return true;
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorPlacedListItem::OnDrag");	
		GetEditor().SelectObject(m_EditorObject);
		m_DragHandler.OnDragStart();
		
		return true;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget receiver)
	{
		EditorLog.Trace("EditorPlacedListItem::OnDrop");
		m_DragHandler.OnDragFinish();
		
		return true;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_EditorObject.GetMarker()) {
			m_EditorObject.GetMarker().Select();
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_EditorObject.GetMarker()) {
			m_EditorObject.GetMarker().Deselect();
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		GetEditor().CommandManager.ObjectPropertiesCommand.Execute(this, null);
		
		return true;
	}
}
