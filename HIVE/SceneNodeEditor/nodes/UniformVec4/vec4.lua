function UniformVec4(arg)
	local name = arg.name
	local val  = arg.val
	return {
		uniform = function()
			return {name=name, value=val, type='vec4'}
		end
	}
end