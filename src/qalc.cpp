#include <qalc.hpp>
#include <util.hpp>

juno::module::registration_result juno::qalc::make_command(dpp::cluster& cluster) const {
    return dpp::slashcommand{ name(), "Use libqalculate!", cluster.me.id }
        .add_option(
            dpp::command_option(dpp::co_string, "expr", "Expression to qalculate", true)
        );
}

void juno::qalc::on_command_execution(const dpp::slashcommand_t& event, dpp::cluster& cluster) const {
    const std::string expr{ std::get<std::string>(event.get_parameter("expr")) };

    PrintOptions print_options{};

    print_options.use_unit_prefixes = false;
    print_options.interval_display = IntervalDisplay::INTERVAL_DISPLAY_MIDPOINT;
    print_options.number_fraction_format = NumberFractionFormat::FRACTION_FRACTIONAL;

    EvaluationOptions evaluation_options{};
    evaluation_options.interval_calculation = IntervalCalculation::INTERVAL_CALCULATION_NONE;

    std::string result{ m_calculator->calculateAndPrint(expr, 10000, evaluation_options, print_options) };

    const dpp::embed reply = dpp::embed{}
                             .set_title("Qalculate!")
                             .add_field(
                                 "Input:",
                                 util::to_codeblock(expr.substr(0, 1022))
                             )
                             .add_field(
                                 "Output:",
                                 util::to_codeblock(result.substr(0, 1022)) // substr to ensure 1024 char lim
                             );

    const dpp::message msg{ event.command.channel_id, reply };

    event.reply(msg);
}
