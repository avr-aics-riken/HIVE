Volren = {}
setmetatable(Volren, {__index = HiveBaseModule})

Volren.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=Volren})
    return this
end

function Volren:Do()
    -- nothing to do
    self:UpdateValue()
    return true
end

function Volren:tf_tex()
    return { name = "tf_tex", type = "texture", value = self.value.tf_tex}
end

function Volren:tf_min()
    return { name = "tf_min", type = "float", value = self.value.tf_min}
end

function Volren:tf_max()
    return { name = "tf_max", type = "float", value = self.value.tf_max}
end

function Volren:u_enableGradientMap()
    return { name = "u_enableGradientMap", type = "float", value = self.value.enableG}
end

function Volren:u_gradTex()
    return { name= "u_gradTex", type='texture', value=self.value.gradTex}
end

function Volren:u_samples()
    return { name = "u_samples", type = "float", value = self.value.samples}
end

function Volren:u_volumeDensity()
    return { name = "u_volumeDensity", type = "float", value = self.value.density}
end

function Volren:u_enableLighting()
    return { name = "u_enableLighting", type = "float", value = self.value.enableL}
end

function Volren:u_light()
    return { name = "u_light", type='vec4', value=self.value.light}
end

function Volren:u_attenuation()
    return { name = "u_attenuation", type='vec3', value=self.value.attenuation}
end

function Volren:u_specular()
    return { name = "u_specular", type='vec3', value=self.value.specular}
end

function Volren:u_ambient()
    return { name = "u_ambient", type='vec3', value=self.value.ambient}
end

function Volren:offset()
    return { name = "offset", type='vec3', value=self.value.offset}
end
