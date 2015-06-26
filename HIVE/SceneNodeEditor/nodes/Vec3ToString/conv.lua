function Vec3ToString(arg)
	local str = 'vec3(' .. tostring(arg.Vec3[1]) .. ',' .. tostring(arg.Vec3[2]) .. ',' .. tostring(arg.Vec3[3]) .. ')'
	return {
		String = function ()
			return str
		end
	}
end