class EditorObjectPropertiesCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorObjectPropertiesDialog properties_dialog("Edit Properties", m_Editor.GetSelectedObjects().GetElement(0));
		properties_dialog.ShowDialog();
	}
	
	override string GetName() 
	{
		return "Properties";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_T };
	}
}