
class FloatsToFloatBuffer : public LuaRef
{
private:
	LuaRefPtr<FloatBuffer>  m_buf;
	
public:
	FloatsToFloatBuffer(){
		m_buf = new FloatBuffer();
	}
	
	int Create(FloatBuffer* buf, int component, int offset){
		if (!buf)
			return 0;
		const int n = buf->GetNum() / component;
		m_buf->Create(n);
		for (int i = 0; i < n; ++i){
			(m_buf->GetBuffer())[i] = buf->GetBuffer()[component * i + offset];
		}
		return buf->GetNum();
	}
	
	int Component()
	{
		return 1;
	}
	
	FloatBuffer* Buffer()
	{
		return m_buf;
	}

	LUA_SCRIPTCLASS_BEGIN(FloatsToFloatBuffer)
	LUA_SCRIPTCLASS_METHOD_ARG3(int,Create,FloatBuffer*,int,int)
	LUA_SCRIPTCLASS_METHOD_ARG0(int,Component)
	LUA_SCRIPTCLASS_METHOD_ARG0(FloatBuffer*,Buffer)
	LUA_SCRIPTCLASS_END()

};
LUA_SCRIPTCLASS_CAST_AND_PUSH(FloatsToFloatBuffer);

