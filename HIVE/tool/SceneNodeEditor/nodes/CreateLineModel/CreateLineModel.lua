function CreateLineModel(property)
	local lm = LineModel();
	lm:Create(property.line);
	lm:SetTranslate(property.translate[1], property.translate[2], property.translate[3])
	lm:SetRotate(property.rotate[1], property.rotate[2], property.rotate[3])
	lm:SetScale(property.scale[1], property.scale[2], property.scale[3])
	lm:SetShader(property.shadername)
	lm:SetLineWidth(property.width)
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
		return lm
	end
	return {
		model=getModel
	}
end
