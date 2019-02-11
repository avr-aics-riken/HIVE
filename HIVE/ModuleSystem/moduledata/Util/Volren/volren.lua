Volren = {}
setmetatable(Volren, {__index = HiveBaseModule})

Volren.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=Volren})
    this.volren = LoadModule('Volren')
    return this
end

function Volren:Do()
    self:UpdateValue()
    
    local val = self.value;
    
    local minval = 0.0
    local maxval = 1.0
    if val.volume then
        if val.volume:Width() > 0 and val.volume:Height() > 0 and val.volume:Depth() > 0 then 
            self.volren:Create(val.volume)
            -- Analyze min/max
            local analyzer = LoadModule('Analyzer').VolumeAnalyzer()
            analyzer:Execute(val.volume)
            minval = analyzer:MinX()
            maxval = analyzer:MaxX()
            print('analyzer: min=', minval, ' max=', maxval)
       end
    end    
    
    self.volren:SetFloat('volumemin', minval)
    self.volren:SetFloat('volumemax', maxval)
    
    if self.volren:GetShader() ~= val.shadername then
        clearShaderCache(val.shadername)
    end
	self.volren:SetShader(val.shadername)

	local uniforms = self.connection.Uniform
    if uniforms ~= nil then    
        for i,v in pairs(uniforms) do
            if v.type == 'vec4' then
                print('vec4[' .. v.name .. '] = (', v.value[1], v.value[2], v.value[3], v.value[4], ')')
                self.volren:SetVec4(v.name, v.value[1], v.value[2], v.value[3], v.value[4])
            end
            if v.type == 'vec3' then
                print('vec3[' .. v.name .. '] = (', v.value[1], v.value[2], v.value[3], ')')
                self.volren:SetVec3(v.name, v.value[1], v.value[2], v.value[3])
            end
            if v.type == 'vec2' then
                print('vec2[' .. v.name .. '] = (', v.value[1], v.value[2], ')')
                self.volren:SetVec2(v.name, v.value[1], v.value[2])
            end
            if v.type == 'float' then
                print('float[' .. v.name .. '] = (', v.value, ')')
                self.volren:SetFloat(v.name, v.value)
            end
            if v.type == 'int' then
                print('int[' .. v.name .. '] = (', v.value, ')')
                self.volren:SetInt(v.name, v.value)
            end
            if v.type == 'texture' then
                print('texture[' .. v.name .. '] = (', v.value, ')')
                self.volren:SetTexture(v.name, v.value)
            end
        end
    end
    
    self.volren:ClearExtraBuffer()
    local varyings = self.connection.Varying
    if varyings ~= nil then
        for i, v in pairs(varyings) do
            self.volren:AddExtraBuffer(v.name, v.value)
        end
    end
    
    if val.tf_tex ~= nil then
        self.volren:SetTransferFunctionTexture(val.tf_tex)
    end
    
    self.volren:SetTransferFunctionMin(val.tf_min)
    self.volren:SetTransferFunctionMax(val.tf_max)
    if val.gradTex ~= nil then
        self.volren:SetGradientTexure(val.gradTex)
    end
    self.volren:EnableGradientMap(val.enableG);
    
    self.volren:EnableLighting(val.enableL);
    self.volren:SetLight(val.light[1], val.light[2], val.light[3], val.light[4]);
    self.volren:SetAttenuation(val.attenuation[1], val.attenuation[2], val.attenuation[3]);
    self.volren:SetAmbient(val.ambient[1], val.ambient[2], val.ambient[3]);
    self.volren:SetSpecular(val.specular[1], val.specular[2], val.specular[3]);
    self.volren:SetSamples(val.samples);
    self.volren:SetOffset(val.offset[1], val.offset[2], val.offset[3]);
    
    return true
end

function Volren:model()
    return self.volren;
end
