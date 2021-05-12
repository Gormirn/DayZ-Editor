class EditorActionStack: set<ref EditorAction>
{
	int InsertAction(EditorAction value)
	{	
		int count = Count();
		for (int i = 0; i < count; i++) {
			if (!this[i].IsUndone()) {
				break;
			}
			
			Remove(i);
			i--; count--;
		}

		// Adds to bottom of stack
		return InsertAt(value, 0);
	}
}

class EditorAction
{
	private string name;
	private bool undone = false;
	
	ref map<int, ref Param> UndoParameters = new map<int, ref Param>();
	ref map<int, ref Param> RedoParameters = new map<int, ref Param>();
	
	string m_UndoAction, m_RedoAction;
			
	void EditorAction(string undo_action, string redo_action)
	{
		name = undo_action;
		m_UndoAction = undo_action;
		m_RedoAction = redo_action;
	}
	
	void ~EditorAction()
	{
		EditorLog.Trace("~EditorAction");
		
		/*foreach (int i, Param p: UndoParameters)
			GetEditor().DeleteSessionData(i);*/
	}
	
	string GetName() 
	{ 
		return name; 
	}
	
	bool IsUndone() 
	{ 
		return undone; 
	}
	
	void CallUndo()
	{
		EditorLog.Trace("EditorAction::CallUndo %1", name);		
		undone = true;
		foreach (int id, Param param: UndoParameters) {
			g_Script.Call(this, m_UndoAction, param);
		}
	}
	
	void CallRedo()
	{
		EditorLog.Trace("EditorAction::CallRedo %1", name);
		undone = false;
		foreach (int id, Param param: RedoParameters) {
			g_Script.Call(this, m_RedoAction, param);
		}
	}
	
	void InsertUndoParameter(Object source, Param params)
	{
		//EditorLog.Trace("InsertUndoParameter %1", source.GetID().ToString());		
		UndoParameters.Insert(source.GetID(), params);
	}	
	
	void InsertUndoParameter(EditorObject source, Param params)
	{
		//EditorLog.Trace("InsertUndoParameter %1", source.GetID().ToString());
		UndoParameters.Insert(source.GetID(), params);
	}	
		
	void InsertRedoParameter(Object source, Param params)
	{
		//EditorLog.Trace("InsertRedoParameter %1", source.GetID().ToString());		
		RedoParameters.Insert(source.GetID(), params);
	}
	
	void InsertRedoParameter(EditorObject source, Param params)
	{
		//EditorLog.Trace("InsertRedoParameter %1", source.GetID().ToString());		
		RedoParameters.Insert(source.GetID(), params);
	}

	void Create(Param1<int> params)
	{
		//EditorLog.Trace("EditorAction::Create %1", params.param1.ToString());
		EditorObjectData data = GetEditor().GetSessionDataById(params.param1);
		if (!data) {
			EditorLog.Error("EditorAction::Create Data was null!");
			return;
		}
		
		GetEditor().CreateObject(data, false);
	}
	
	void Delete(Param1<int> params)
	{
		//EditorLog.Trace("EditorAction::Delete %1", params.param1.ToString());
		EditorObject object = GetEditor().GetPlacedObjectById(params.param1);
		if (!object) {
			EditorLog.Error("EditorAction::Delete Object was null!");
			return;
		}
		
		GetEditor().DeleteObject(object, false);
	}
	
	void SetTransform(Param3<int, vector, vector> params)
	{
		//EditorLog.Trace("EditorAction::SetTransform");
		EditorObjectData editor_object_data = GetEditor().GetSessionDataById(params.param1);
		if (!editor_object_data) {
			EditorLog.Error("EditorAction::SetTransform EditorObjectData was null!");
			return;
		}
		
		EditorObject editor_object = GetEditor().GetPlacedObjectById(editor_object_data.GetID());
		if (!editor_object) {
			EditorLog.Error("EditorAction::SetTransform EditorObject was null!");
			return;
		}

		editor_object.SetPosition(params.param2);
		editor_object.SetOrientation(params.param3);
	}
	
	void Hide(Param1<Object> param)
	{
		//EditorLog.Trace("EditorAction::Hide %1", param.param1.ToString());
		CF.ObjectManager.HideMapObject(param.param1);
	}
	
	void Unhide(Param1<Object> param)
	{
		//EditorLog.Trace("EditorAction::Unhide %1", param.param1.ToString());
		CF.ObjectManager.UnhideMapObject(param.param1);
	}
	
	void Lock(Param1<EditorObject> param)
	{
		param.param1.Lock(true);
	}
	
	void Unlock(Param1<EditorObject> param)
	{
		param.param1.Lock(false);
	}
}