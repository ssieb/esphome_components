import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins, automation
from esphome.components import uart
from esphome.components import text_sensor
from esphome.const import CONF_ID, CONF_ON_TAG, CONF_TRIGGER_ID

MULTI_CONF = True

wiegand_ns = cg.esphome_ns.namespace('wiegand')

Wiegand = wiegand_ns.class_('Wiegand', cg.Component)
WiegandStrTrigger = wiegand_ns.class_(
    "WiegandStrTrigger", automation.Trigger.template(cg.std_string)
)
WiegandDigitTrigger = wiegand_ns.class_(
    "WiegandDigitTrigger", automation.Trigger.template(cg.uint8)
)

CONF_D0 = "d0"
CONF_D1 = "d1"
CONF_ON_DIGIT = "on_digit"
CONF_ON_PIN = "on_pin"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Wiegand),
        cv.Required(CONF_D0): pins.internal_gpio_input_pin_schema,
        cv.Required(CONF_D1): pins.internal_gpio_input_pin_schema,
        cv.Optional(CONF_ON_TAG): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(WiegandStrTrigger),
            }
        ),
        cv.Optional(CONF_ON_DIGIT): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(WiegandDigitTrigger),
            }
        ),
        cv.Optional(CONF_ON_PIN): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(WiegandStrTrigger),
            }
        ),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    pin = await cg.gpio_pin_expression(config[CONF_D0])
    cg.add(var.set_d0_pin(pin))
    pin = await cg.gpio_pin_expression(config[CONF_D1])
    cg.add(var.set_d1_pin(pin))

    for conf in config.get(CONF_ON_TAG, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID])
        cg.add(var.register_tag_trigger(trigger))
        await automation.build_automation(trigger, [(cg.std_string, "x")], conf)

    for conf in config.get(CONF_ON_DIGIT, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID])
        cg.add(var.register_digit_trigger(trigger))
        await automation.build_automation(trigger, [(cg.uint8, "x")], conf)

    for conf in config.get(CONF_ON_PIN, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID])
        cg.add(var.register_pin_trigger(trigger))
        await automation.build_automation(trigger, [(cg.std_string, "x")], conf)

