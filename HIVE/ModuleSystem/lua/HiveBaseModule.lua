
local HiveBaseModule = {}

HiveBaseModule.new = function (varname)
    local this = {}
    this.varname = varname
    this.property = {}
    this.connection = {}
    this.value = {}
    setmetatable(this, {__index = HiveBaseModule})
    return this;
end

function HiveBaseModule:ClearConnect()
    self.connection = {}
end

local function checkArray(name)
      local p = string.find(name, "%[%d+%]")
      if p == nil then
         return false
      end
      local arrayName = string.sub(name, 0, p-1)
      local idx = string.sub(name, p+1, -2)
      return true, arrayName, idx
end

function HiveBaseModule:Connect(propname, value)
    local r, arrayname, index
    r, arrayname, index = checkArray(propname)
    if r == false then
        self.connection[propname] = value
    else
        local carr = self.connection[arrayname] 
        if carr then
            carr[#carr + 1] = value
        else
            self.connection[arrayname] = {value}
        end     
    end        
end

function HiveBaseModule:Set(propname, value)
    self.property[propname] = value
end

function HiveBaseModule:Do()    
    print('[Warning:HiveBaseModule][LUA] Need to override Do() method.')
end

function HiveBaseModule:UpdateValue()
    self.value = {}
    for i, v in pairs(self.property) do
        self.value[i] = v;
    end
    for i, v in pairs(self.connection) do
        self.value[i] = v;
    end
end

function HiveBaseModule:PrintValue()
    for i, v in pairs(self.property) do
        print('property['..i..']=',v)
    end
    for i, v in pairs(self.connection) do
        print('connect['..i..']=',v)
    end
end

return HiveBaseModule