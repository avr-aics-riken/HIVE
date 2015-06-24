function FloatToString(arg)
	local str = tostring(arg.Float)
	return {
		String = function ()
			return str
		end
	}
end