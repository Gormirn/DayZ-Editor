class EditorPlaceableListItem: EditorListItem
{
	//! Animals and Zombies / Players "survivors"
	static const ref TStringArray TOOLTIP_BLACKLIST = {
		"DZ_LightAI",
		"Man",
		"Car"
	};
	
	protected ref EditorPlaceableItem m_PlaceableItem;
	
	void EditorPlaceableListItem(EditorPlaceableItem placeable_item)
	{
		m_PlaceableItem = placeable_item;
		
		ListItemShow.Show(false);
		m_TemplateController.Label = placeable_item.Type;
		m_TemplateController.NotifyPropertyChanged("Label");
		
		m_TemplateController.Icon = placeable_item.Icon;
		m_TemplateController.NotifyPropertyChanged("Icon");
		
#ifndef COMPONENT_SYSTEM
		EditorEvents.OnStartPlacing.Insert(OnStartPlacing);
		EditorEvents.OnStopPlacing.Insert(OnStopPlacing);
#endif
		
		GetLayoutRoot().ClearFlags(WidgetFlags.DRAGGABLE);
	}
	
	override bool IsSelected() 
	{
		if (GetEditor().ObjectInHand) {
			return (EditorHologram.Cast(GetEditor().ObjectInHand).GetPlaceableItem() == m_PlaceableItem);
		}
		
		return false;
	}
	
	EditorPlaceableItem GetPlaceableItem()
	{
		return m_PlaceableItem;
	}
	
	bool ListItemExecute(ButtonCommandArgs args)
	{
		switch (args.GetMouseButton()) {

			case 0: {
				GetEditor().CreateInHand(m_PlaceableItem);
				Select();
				break;
			}
			
			case 1: {
				Select();
				int x, y;
				GetMousePos(x, y);
				EditorHud.CurrentMenu = new EditorPlaceableContextMenu(x, y);
				break;
			}
		}
				
		return true;
	}
	
	void OnStartPlacing(Class context, EditorPlaceableItem placeable_item)
	{
		if (placeable_item == m_PlaceableItem) {
			Select();
			return;
		}

		Deselect();
	}
	
	void OnStopPlacing(Class context)
	{
		Deselect();
	}
	
	override void Select()
	{
		super.Select();
		GetEditor().GetObjectManager().CurrentSelectedItem = m_PlaceableItem;
	}
	
	override void Deselect()
	{
		super.Deselect();
		//delete GetEditor().GetObjectManager().CurrentSelectedItem;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		EditorTooltip tooltip = new EditorTooltip();
		
		float size_x, size_y, pos_x, pos_y;
		m_LayoutRoot.GetScreenPos(pos_x, pos_y);
		m_LayoutRoot.GetScreenSize(size_x, size_y);
		
		tooltip.SetTitle(m_PlaceableItem.Type);	
		tooltip.SetPosition(pos_x + size_x, pos_y);
		
		//! bugfix
		GetEditor().GetObjectManager().CurrentSelectedItem = m_PlaceableItem;
		
		if (m_PlaceableItem && !IsBlacklistedItem(m_PlaceableItem.Type)) {
			tooltip.SetContent(GetGame().CreateObjectEx(m_PlaceableItem.Type, Vector(0, -1000, 0), ECE_NONE));
		}		
		
		EditorHud.SetCurrentTooltip(tooltip);
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		EditorHud.SetCurrentTooltip(null);
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorPlaceableListItem::OnDrag");	
		if (!GetEditor().IsPlacing()) {
			GetEditor().CreateInHand(m_PlaceableItem);
		}
		
		return true;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget receiver)
	{
		EditorLog.Trace("EditorPlaceableListItem::OnDrop");
		if (GetEditor().IsPlacing()) {
			GetEditor().CommandManager[EditorPlaceObjectCommand].Execute(this, null);
		}
		
		return true;
	}
		
	bool FilterType(string filter)
	{
		if (filter == string.Empty) return true;
		
		string type_lower = m_PlaceableItem.Type;
		type_lower.ToLower();
		filter.ToLower();
		
		if (filter[0] == "@") {
			type_lower = m_PlaceableItem.Mod.GetModName();
			filter[0] = "";
			type_lower.ToLower();
		}
		
		return type_lower.Contains(filter);
	}
	
	bool IsBlacklistedItem(string item_type)
	{
		foreach (string blacklist_check: TOOLTIP_BLACKLIST) {
			if (GetGame().IsKindOf(item_type, blacklist_check)) {
				return true;
			}
		}
		
		return false;
	}
}
