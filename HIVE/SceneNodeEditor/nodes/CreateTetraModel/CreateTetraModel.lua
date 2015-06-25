function CreateTetraModel(property)
	local tm = TetraModel();
	tm:Create(property.tetra);
	tm:SetTranslate(property.translate[1], property.translate[2], property.translate[3])
	tm:SetRotate(property.rotate[1], property.rotate[2], property.rotate[3])
	tm:SetScale(property.scale[1], property.scale[2], property.scale[3])
	tm:SetShader(property.shadername)

	local uniforms = property.Uniform
	for i,v in pairs(uniforms) do
		if v.type == 'vec4' then
			print('vec4[' .. v.name .. '] = (', v.value[1], v.value[2], v.value[3], v.value[4], ')')
			tm:SetVec4(v.name, v.value[1], v.value[2], v.value[3], v.value[4])
		end
		if v.type == 'vec3' then
			print('vec3[' .. v.name .. '] = (', v.value[1], v.value[2], v.value[3], ')')
			tm:SetVec3(v.name, v.value[1], v.value[2], v.value[3])
		end
		if v.type == 'vec2' then
			print('vec2[' .. v.name .. '] = (', v.value[1], v.value[2], ')')
			tm:SetVec2(v.name, v.value[1], v.value[2])
		end
		if v.type == 'float' then
			print('float[' .. v.name .. '] = (', v.value, ')')
			tm:SetFloat(v.name, v.value)
		end
		if v.type == 'texture' then
			print('texture[' .. v.name .. '] = (', v.value, ')')
			tm:SetTexture(v.name, v.value)
		end
	end
	function getModel()
		return tm
	end
	return {
		model=getModel
	}
end
