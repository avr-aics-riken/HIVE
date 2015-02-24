function CreateVectorModel(property)
	local vm = VectorModel();
	vm:Create(property.vector);
	vm:SetShader(property.shadername)
	vm:SetLineWidth(property.linewidth)
	vm:SetArrowSize(property.arrowsize)
	vm:SetLengthScale(property.lengthscale)
	local uniforms = property.Uniform
	for i,v in pairs(uniforms) do
		if v.type == 'vec4' then
			print('vec4[' .. v.name .. '] = (', v.value[1], v.value[2], v.value[3], v.value[4], ')')
			lm:SetVec4(v.name, v.value[1], v.value[2], v.value[3], v.value[4])
		end
		if v.type == 'vec3' then
			print('vec3[' .. v.name .. '] = (', v.value[1], v.value[2], v.value[3], ')')
			lm:SetVec3(v.name, v.value[1], v.value[2], v.value[3])
		end
		if v.type == 'vec2' then
			print('vec2[' .. v.name .. '] = (', v.value[1], v.value[2], ')')
			lm:SetVec2(v.name, v.value[1], v.value[2])
		end
		if v.type == 'float' then
			print('float[' .. v.name .. '] = (', v.value, ')')
			lm:SetFloat(v.name, v.value)
		end
	end
	function getModel()
		return vm
	end
	return {
		model = getModel
	}
end
