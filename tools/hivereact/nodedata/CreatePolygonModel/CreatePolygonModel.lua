CreatePolygonModel = {}
setmetatable(CreatePolygonModel, {__index = BaseComponent})

CreatePolygonModel.new = function (varname)
    local this = BaseComponent.new(varname)
    this.property = {
        mesh = nil,
        translate = {0,0,0},
        rotate = {0,0,0},
        scale = {0,0,0},
        shadername = 'polygon.frag',
        Uniform = {
            
        }
    }

    setmetatable(this, {__index=CreatePolygonModel})
    return this
end

function CreatePolygonModel:Do()
    local pm = PolygonModel();
    local property = self.property
    self.pmodel = pm
    if self.connection.mesh then
	   pm:Create(self.connection.mesh)
    end
	pm:SetTranslate(property.translate[1], property.translate[2], property.translate[3])
	pm:SetRotate(property.rotate[1], property.rotate[2], property.rotate[3])
	pm:SetScale(property.scale[1], property.scale[2], property.scale[3])
	pm:SetShader(property.shadername)

	local uniforms = property.Uniform
	for i,v in pairs(uniforms) do
		if v.type == 'vec4' then
			print('vec4[' .. v.name .. '] = (', v.value[1], v.value[2], v.value[3], v.value[4], ')')
			pm:SetVec4(v.name, v.value[1], v.value[2], v.value[3], v.value[4])
		end
		if v.type == 'vec3' then
			print('vec3[' .. v.name .. '] = (', v.value[1], v.value[2], v.value[3], ')')
			pm:SetVec3(v.name, v.value[1], v.value[2], v.value[3])
		end
		if v.type == 'vec2' then
			print('vec2[' .. v.name .. '] = (', v.value[1], v.value[2], ')')
			pm:SetVec2(v.name, v.value[1], v.value[2])
		end
		if v.type == 'float' then
			print('float[' .. v.name .. '] = (', v.value, ')')
			pm:SetFloat(v.name, v.value)
		end
		if v.type == 'texture' then
			print('texture[' .. v.name .. '] = (', v.value, ')')
			pm:SetTexture(v.name, v.value)
		end
	end
end

function CreatePolygonModel:model()
    return self.pmodel;
end
