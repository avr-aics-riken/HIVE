function UniformFloat(arg)
	local name = arg.name
	local val  = arg.val
	return {
		uniform = function()
			return {name=name, value=val, type='float'}
		end
	}
end