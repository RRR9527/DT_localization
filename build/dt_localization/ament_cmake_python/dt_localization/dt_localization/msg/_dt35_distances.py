# generated from rosidl_generator_py/resource/_idl.py.em
# with input from dt_localization:msg/Dt35Distances.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

# Member 'distance'
import numpy  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_Dt35Distances(type):
    """Metaclass of message 'Dt35Distances'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('dt_localization')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'dt_localization.msg.Dt35Distances')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__dt35_distances
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__dt35_distances
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__dt35_distances
            cls._TYPE_SUPPORT = module.type_support_msg__msg__dt35_distances
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__dt35_distances

            from std_msgs.msg import Header
            if Header.__class__._TYPE_SUPPORT is None:
                Header.__class__.__import_type_support__()

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class Dt35Distances(metaclass=Metaclass_Dt35Distances):
    """Message class 'Dt35Distances'."""

    __slots__ = [
        '_header',
        '_distance',
        '_valid',
        '_frame_count',
        '_crc_error_count',
    ]

    _fields_and_field_types = {
        'header': 'std_msgs/Header',
        'distance': 'float[4]',
        'valid': 'boolean[4]',
        'frame_count': 'uint32',
        'crc_error_count': 'uint32',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.NamespacedType(['std_msgs', 'msg'], 'Header'),  # noqa: E501
        rosidl_parser.definition.Array(rosidl_parser.definition.BasicType('float'), 4),  # noqa: E501
        rosidl_parser.definition.Array(rosidl_parser.definition.BasicType('boolean'), 4),  # noqa: E501
        rosidl_parser.definition.BasicType('uint32'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint32'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        from std_msgs.msg import Header
        self.header = kwargs.get('header', Header())
        if 'distance' not in kwargs:
            self.distance = numpy.zeros(4, dtype=numpy.float32)
        else:
            self.distance = kwargs.get('distance')
        self.valid = kwargs.get(
            'valid',
            [bool() for x in range(4)]
        )
        self.frame_count = kwargs.get('frame_count', int())
        self.crc_error_count = kwargs.get('crc_error_count', int())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.header != other.header:
            return False
        if any(self.distance != other.distance):
            return False
        if self.valid != other.valid:
            return False
        if self.frame_count != other.frame_count:
            return False
        if self.crc_error_count != other.crc_error_count:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def header(self):
        """Message field 'header'."""
        return self._header

    @header.setter
    def header(self, value):
        if __debug__:
            from std_msgs.msg import Header
            assert \
                isinstance(value, Header), \
                "The 'header' field must be a sub message of type 'Header'"
        self._header = value

    @builtins.property
    def distance(self):
        """Message field 'distance'."""
        return self._distance

    @distance.setter
    def distance(self, value):
        if isinstance(value, numpy.ndarray):
            assert value.dtype == numpy.float32, \
                "The 'distance' numpy.ndarray() must have the dtype of 'numpy.float32'"
            assert value.size == 4, \
                "The 'distance' numpy.ndarray() must have a size of 4"
            self._distance = value
            return
        if __debug__:
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 len(value) == 4 and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -3.402823466e+38 or val > 3.402823466e+38) or math.isinf(val) for val in value)), \
                "The 'distance' field must be a set or sequence with length 4 and each value of type 'float' and each float in [-340282346600000016151267322115014000640.000000, 340282346600000016151267322115014000640.000000]"
        self._distance = numpy.array(value, dtype=numpy.float32)

    @builtins.property
    def valid(self):
        """Message field 'valid'."""
        return self._valid

    @valid.setter
    def valid(self, value):
        if __debug__:
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 len(value) == 4 and
                 all(isinstance(v, bool) for v in value) and
                 True), \
                "The 'valid' field must be a set or sequence with length 4 and each value of type 'bool'"
        self._valid = value

    @builtins.property
    def frame_count(self):
        """Message field 'frame_count'."""
        return self._frame_count

    @frame_count.setter
    def frame_count(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'frame_count' field must be of type 'int'"
            assert value >= 0 and value < 4294967296, \
                "The 'frame_count' field must be an unsigned integer in [0, 4294967295]"
        self._frame_count = value

    @builtins.property
    def crc_error_count(self):
        """Message field 'crc_error_count'."""
        return self._crc_error_count

    @crc_error_count.setter
    def crc_error_count(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'crc_error_count' field must be of type 'int'"
            assert value >= 0 and value < 4294967296, \
                "The 'crc_error_count' field must be an unsigned integer in [0, 4294967295]"
        self._crc_error_count = value
