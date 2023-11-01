import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import CONF_ID
from .. import CONF_PHILIPS_ID, philips_series_3200_ns, PhilipsSeries3200

DEPENDENCIES = ['philips_espresso']

CONF_ACTION = 'action'

ActionButton = philips_series_3200_ns.class_("ActionButton", button.Button)

Action = philips_series_3200_ns.enum("Action")

ACTIONS = {
    "MAKE_COFFEE": Action.MAKE_COFFEE,
    "SELECT_COFFEE": Action.SELECT_COFFEE,
    "SELECT_ESPRESSO": Action.SELECT_ESPRESSO,
    "MAKE_ESPRESSO": Action.MAKE_ESPRESSO,
    "SELECT_AMERICANO": Action.SELECT_AMERICANO,
    "MAKE_AMERICANO": Action.MAKE_AMERICANO,
    "SELECT_CAPPUCCINO": Action.SELECT_CAPPUCCINO,
    "MAKE_CAPPUCCINO": Action.MAKE_CAPPUCCINO,
    "SELECT_LATTE": Action.SELECT_LATTE,
    "MAKE_LATTE": Action.MAKE_LATTE,
    "SELECT_HOT_WATER": Action.SELECT_HOT_WATER,
    "MAKE_HOT_WATER": Action.MAKE_HOT_WATER,
    "BEAN_AMOUNT": Action.SELECT_BEAN_AMOUNT,
    "WATER_AMOUNT": Action.SELECT_WATER_AMOUNT,
    "FROTHED_MILK_AMOUNT": Action.SELECT_FROTHED_MILK_AMOUNT,
    "AQUA_CLEAN": Action.SELECT_AQUA_CLEAN,
    "CALC_CLEAN": Action.SELECT_CALC_CLEAN,
    "PLAY_PAUSE": Action.PLAY_PAUSE,
}

CONFIG_SCHEMA = button.button_schema(ActionButton).extend(
    {
        cv.Required(CONF_PHILIPS_ID): cv.use_id(PhilipsSeries3200),
        cv.Required(CONF_ACTION): cv.enum(
            ACTIONS, upper=True, space="_"
        )
    }
)


async def to_code(config):
    var = await button.new_button(config)
    await cg.register_parented(var, config[CONF_PHILIPS_ID])
    cg.add(var.set_action(config[CONF_ACTION]))
