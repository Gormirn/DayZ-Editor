class EditorEnvironmentControlCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorEnvironmentDialog dialog = new EditorEnvironmentDialog("Weather Controller");
		DialogResult result = dialog.ShowDialog();
	}
	
	override string GetName() 
	{
		return "Environment";
	}
	
	override string GetIcon() 
	{
		return "set:dayz_gui image:icon_sun";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_LSHIFT, KeyCode.KC_I };
	}
}