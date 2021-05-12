class EditorWorldObject
{
	protected Object m_WorldObject;
	Object GetWorldObject() {
		return m_WorldObject;
	}
	
	void ~EditorWorldObject()
	{
		if (m_WorldObject) {
			GetGame().ObjectDelete(m_WorldObject);
		}
	}
	
	static Object CreateObject(string type, vector position = "0 0 0", vector orientation = "0 0 0", float scale = 1)
	{
		// Set to ECE_SETUP for AI compat. DONT ADD ECE_LOCAL
		type = type.Trim();
		if (type == string.Empty) return null;
		
		Object obj;	
		if (!Class.CastTo(obj, GetGame().CreateObjectEx(type, position, ECE_SETUP | ECE_CREATEPHYSICS))) {
			EditorLog.Error("EditorWorldObject: Invalid Object %1", type);
			return null;
		}
/*
		if (obj.IsInherited(DayZCreatureAI)) {
			DayZCreatureAI creature;
			if (Class.CastTo(creature, obj)) {
				Print(creature.GetAIAgent());
				obj.DisableSimulation(true);
				obj.DisableSimulation(false);
				Print(creature.GetAIAgent());
				
				creature.InitAIAgent(null);
			}
		}
		*/
		// Needed for AI Placement			
		if (EntityAI.Cast(obj)) {
			EntityAI.Cast(obj).DisableSimulation(true);
		}
		obj.SetOrientation(orientation);
		obj.SetScale(scale);
		obj.Update();
		//obj.SetFlags(EntityFlags.STATIC, true);
		return obj;
	}
}