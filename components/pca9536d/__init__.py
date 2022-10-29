import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import i2c
from esphome.const import (
    CONF_ID,
    CONF_INPUT,
    CONF_INVERTED,
    CONF_NUMBER,
    CONF_MODE,
    CONF_OUTPUT,
)

DEPENDENCIES = ["i2c"]
MULTI_CONF = True

CONF_PCA9536D = "pca9536d"

pca9536d_ns = cg.esphome_ns.namespace("pca9536d")
PCA9536DGPIOMode = pca9536d_ns.enum("PCA9536DGPIOMode")

PCA9536D = pca9536d_ns.class_("PCA9536D", cg.Component, i2c.I2CDevice)
PCA9536DGPIOPin = pca9536d_ns.class_("PCA9536DGPIOPin", cg.GPIOPin)

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.Required(CONF_ID): cv.declare_id(PCA9536D),
    }
).extend(i2c.i2c_device_schema(0x41))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

def validate_mode(value):
    if not (value[CONF_INPUT] or value[CONF_OUTPUT]):
        raise cv.Invalid("Mode must be either input or output")
    if value[CONF_INPUT] and value[CONF_OUTPUT]:
        raise cv.Invalid("Mode must be either input or output")
    return value

PCA9536D_PIN_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(PCA9536DGPIOPin),
        cv.Required(CONF_PCA9536D): cv.use_id(PCA9536D),
        cv.Required(CONF_NUMBER): cv.int_range(min=0, max=3),
        cv.Optional(CONF_MODE, default={}): cv.All(
            {
                cv.Optional(CONF_INPUT, default=False): cv.boolean,
                cv.Optional(CONF_OUTPUT, default=False): cv.boolean,
            },
            validate_mode,
        ),
        cv.Optional(CONF_INVERTED, default=False): cv.boolean,
    }
)

@pins.PIN_SCHEMA_REGISTRY.register(CONF_PCA9536D, PCA9536D_PIN_SCHEMA)
async def pca9536d_pin_to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    parent = await cg.get_variable(config[CONF_PCA9536D])

    cg.add(var.set_parent(parent))
    cg.add(var.set_pin(config[CONF_NUMBER]))
    cg.add(var.set_inverted(config[CONF_INVERTED]))
    cg.add(var.set_flags(pins.gpio_flags_expr(config[CONF_MODE])))
    return var

