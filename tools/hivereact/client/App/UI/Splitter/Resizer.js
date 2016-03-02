'use strict';

Object.defineProperty(exports, "__esModule", {
    value: true
});

var _react = require('react');

var _react2 = _interopRequireDefault(_react);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

exports.default = _react2.default.createClass({
    displayName: 'Resizer',
    onMouseDown: function onMouseDown(event) {
        this.props.onMouseDown(event);
    },
    render: function render() {
        var split = this.props.split;
        var classes = ['Resizer', split];
        return _react2.default.createElement('span', { className: classes.join(' '), onMouseDown: this.onMouseDown });
    }
});
module.exports = exports['default'];