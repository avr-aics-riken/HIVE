
KVS = {}
setmetatable(KVS, {__index = HiveBaseModule})

KVS.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.render = nil
    setmetatable(this, {__index=KVS})
    return this
end

function KVS:Do()
    self.render = LoadModule("RenderPluginKVS")
    if self.render then
        return true
    else
        return false
    end
end

function KVS:renderer()
    return self.render;
end
