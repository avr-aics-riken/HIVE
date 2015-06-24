function UniformVec2(arg)
	local name = arg.name
	local val  = arg.val
	return {
		uniform = function()
			return {name=name, value=val, type='vec2'}
		end
	}
end