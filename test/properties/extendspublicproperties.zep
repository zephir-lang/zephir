
namespace Test\Properties;

class ExtendsPublicProperties extends PublicProperties
{
	public testPropertyFromClassNameConstantValue = PublicProperties::TEST_CONST;

	protected someGetterSetterArray = ["holy"];
}
